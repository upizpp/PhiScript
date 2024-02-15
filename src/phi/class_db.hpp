#pragma once

#include <functional>
#include <phi/exception.hpp>
#include <phi/flag.hpp>
#include <phi/optional.hpp>
#include <phi/singleton.hpp>
#include <phi/traits.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

#define PASS(x) x
#define TO_STRING(x) #x
#define METHOD_R(name, rename)                                                 \
    Singleton<ClassDB>::instance()->registerMethod(#rename, &CLASS::name);
#define METHOD(name) METHOD_R(name, name)
#define PROPERTY_OFFSET(class, name) size_t(&((class *)0)->name)
#define CLASS
#define PROPERTY_R(name, rename)                                               \
    Singleton<ClassDB>::instance()                                             \
        ->registerProperty<decltype(((CLASS *)0)->name)>(                      \
            #rename, PROPERTY_OFFSET(CLASS, name))
#define PROPERTY(name) PROPERTY_R(name, name)
#define PhiClass(class_name, parent_class)                                     \
  public:                                                                      \
    static void static_register();                                             \
    static string className() { return TO_STRING(class_name); }                \
    static string parentClassName() { return TO_STRING(parent_class); }        \
    virtual string getClass() const override { return TO_STRING(class_name); } \
    virtual string getParentClass() const override {                           \
        return TO_STRING(parent_class);                                        \
    }                                                                          \
                                                                               \
  private:                                                                     \
    ClassRegister<PASS(class_name), PASS(parent_class)> _;

namespace phi {
    using callable_t = std::function<Ref<Variant>(const array &)>;

    template <typename> inline constexpr bool is_vector_v = false;
    template <typename T, typename A>
    inline constexpr bool is_vector_v<vector<T, A>> = true;

    struct ClassInfo {
        using type = Variant;
        using arg_list = vector<type>;
        using method = std::function<type(Object *, const array &)>;
        using initializer_t = std::function<Object *(void)>;

        ClassInfo *parent;
        map<string, method> methods;
        initializer_t initializer;
        map<string, std::pair<std::function<type(Object *)>,
                              std::function<void(Object *, const type &)>>>
            properties;

        ClassInfo() {}
        ClassInfo(ClassInfo *p, const initializer_t &init)
            : parent(p), initializer(init) {}

        type call(Object *, const string &, const array &);

        bool hasProperty(const string &);
        bool hasMethod(const string &);
        method &getMethod(const string &);
    };

    class ClassDB {
      public:
        using type = Variant;
        using arg_list = vector<type>;

      private:
        map<string, ClassInfo> _M_classes;
        string _M_bound;
        string _M_calling;

      public:
        void bind(const string &class_name) { _M_bound = class_name; }
        void registerClass(const ClassInfo::initializer_t &initializer,
                           const string &class_name, const string &parent) {
            ClassInfo *parent_info = _M_classes.find(parent) != _M_classes.end()
                                         ? &_M_classes[parent]
                                         : nullptr;
            _M_classes.insert(
                {class_name, ClassInfo{parent_info, initializer}});
            bind(class_name);
        }

        template <typename F>
        void registerMethod(const string &name, const F &func) {
            auto caller = [=](Object *obj, const array &args) -> type {
                return *toCallable(obj, func)(args);
            };
            getInfo().methods.insert({name, caller});
        }
        template <typename T>
        void registerProperty(const string &name, size_t offset) {
            auto setter = [=](Object *obj, const type &value) -> void {
                char *addr = (char *)obj + offset;
                *reinterpret_cast<T *>(addr) = static_cast<T>(value);
            };
            auto getter = [=](Object *obj) -> type {
                char *addr = (char *)obj + offset;
                return type{*reinterpret_cast<T *>(addr)};
            };
            getInfo().properties.insert({name, {getter, setter}});
        }

      private:
        struct ArgumentHandler {
            size_t requiredCount;

            template <typename T> T handle(const array &args, size_t index) {
                return HandleImpl<T>()(requiredCount, args, index);
            }

          private:
            template <typename T> struct HandleImpl {
                T operator()(size_t requiredCount, const array &args,
                             size_t index) {
                    if (index >= args.size())
                        throw ArgumentException(
                            requiredCount, args.size(),
                            Singleton<ClassDB>::instance()->_M_calling);
                    return args[index]->convertTo(VariantType<T>::value);
                }
            };
            template <typename T> struct HandleImpl<T &> {
                T &operator()(size_t requiredCount, const array &args,
                              size_t index) {
                    if (index >= args.size())
                        throw ArgumentException(
                            requiredCount, args.size(),
                            Singleton<ClassDB>::instance()->_M_calling);
                    return args[index]->seeAs<T>();
                }
            };
            template <typename T> struct HandleImpl<const T &> {
                const T &operator()(size_t requiredCount, const array &args,
                                    size_t index) {
                    if (index >= args.size())
                        throw ArgumentException(
                            requiredCount, args.size(),
                            Singleton<ClassDB>::instance()->_M_calling);
                    return args[index]->seeAs<T>();
                }
            };
            template <typename T> struct HandleImpl<Optional<T>> {
                Optional<T> operator()(size_t requiredCount, const array &args,
                                       size_t index) {
                    if (index >= args.size())
                        return Optional<T>(nullptr);
                    return args[index]->seeAs<T>();
                }
            };
            template <typename T> struct HandleImpl<Optional<const T>> {
                Optional<const T> operator()(size_t requiredCount,
                                             const array &args, size_t index) {
                    if (index >= args.size())
                        return Optional<const T>(nullptr);
                    return args[index]->seeAs<T>();
                }
            };
            template <typename T> struct HandleImpl<OptionalRef<T>> {
                OptionalRef<T> operator()(size_t requiredCount,
                                          const array &args, size_t index) {
                    if (index >= args.size())
                        return OptionalRef<T>(nullptr);
                    return const_cast<T *>(&args[index]->seeAs<T>());
                }
            };
            template <typename T> struct HandleImpl<OptionalRef<const T>> {
                OptionalRef<const T> operator()(size_t requiredCount,
                                                const array &args,
                                                size_t index) {
                    if (index >= args.size())
                        return OptionalRef<const T>(nullptr);
                    return &args[index]->seeAs<T>();
                }
            };
        };

        friend struct ArgumentHandler;
        friend struct call_impl;

        template <typename I, typename F> struct call_impl;

        template <size_t... I, typename R, typename... Args>
        struct call_impl<index_seq<I...>, R(Args...)> {
            using F = R(Args...);
            static Ref<Variant> call(const std::function<F> f,
                                     const array &args) {
                if constexpr (std::is_same_v<R, void>) {
                    ArgumentHandler handler;
                    handler.requiredCount = sizeof...(I);
                    f(handler.handle<
                        typename function_traits<F>::template args<I>::type>(
                        args, I)...);
                    return Variant::Null;
                } else if constexpr (std::is_same_v<R, Ref<Variant>>) {
                    ArgumentHandler handler;
                    handler.requiredCount = sizeof...(I);
                    return f(handler.handle<typename function_traits<
                                 F>::template args<I>::type>(args, I)...);
                } else if constexpr (is_vector_v<R> &&
                                     !std::is_same_v<R, array>) {
                    ArgumentHandler handler;
                    handler.requiredCount = sizeof...(I);
                    R temp = f(handler.handle<typename function_traits<
                                   F>::template args<I>::type>(args, I)...);
                    Owner<array> result{new array{temp.size(), nullptr}};
                    for (size_t i = 0; i < temp.size(); ++i)
                        (*result)[i] = new Variant{temp[i]};
                    return new Variant{std::move(result)};
                } else {
                    ArgumentHandler handler;
                    handler.requiredCount = sizeof...(I);
                    return new Variant{
                        f(handler.handle<typename function_traits<
                              F>::template args<I>::type>(args, I)...)};
                }
            }
        };

        template <typename F> struct class_call_impl;

        template <typename T, typename R, typename... Args>
        struct class_call_impl<R (T::*)(Args...)> {
            static Ref<Variant> call(Object *obj, R (T::*f)(Args...),
                                     const array &args) {
                using seq = gen_index_seq_t<sizeof...(Args)>;
                return call_impl<seq, R(Args...)>::call(
                    [&](Args... args) -> R {
                        return (((T *)obj)->*f)(args...);
                    },
                    args);
            }
        };

      public:
        template <typename F> static callable_t toCallable(const F &func) {
            return [=](const array &args) -> Ref<Variant> {
                using seq = gen_index_seq_t<function_traits<F>::arity>;
                return call_impl<seq, F>::call(func, args);
            };
        }
        template <typename F>
        static callable_t toCallable(Object *obj, const F &func) {
            return [=](const array &args) -> Ref<Variant> {
                return class_call_impl<F>::call(obj, func, args);
            };
        }
        static callable_t toCallable(Object *obj, const string &func) {
            return [=](const array &args) -> Ref<Variant> {
                return new Variant{
                    Singleton<ClassDB>::instance()->call(obj, func, args)};
            };
        }

        ClassInfo &getInfo() { return _M_classes[_M_bound]; }

        type call(Object *obj, const string &method_name,
                  const array &args = {});
        void set(Object *obj, const string &property_name, const type &value);
        type get(Object *obj, const string &property_name);
        bool hasProperty(const Object *obj, const string &property_name);
        bool hasMethod(const Object *obj, const string &method_name);
        Object *create(const string &class_name);

        ClassInfo &parent(const string &class_name) {
            return *_M_classes[class_name].parent;
        }
    };

    template <> struct ClassDB::ArgumentHandler::HandleImpl<RestParameters> {
        RestParameters operator()(size_t requiredCount, const array &args,
                                  size_t index) {
            array rest_args;
            size_t rest = args.size() - index;
            rest_args.resize(rest);
            for (size_t i = index; i < args.size(); i++)
                rest_args[i - index] = args[i];
            return RestParameters{std::move(rest_args)};
        }
    };
    template <> struct ClassDB::ArgumentHandler::HandleImpl<Ref<Variant>> {
        Ref<Variant> operator()(size_t requiredCount, const array &args,
                                size_t index) {
            return args[index];
        }
    };

    template <typename T, typename P> struct ClassRegister {
        ClassRegister() {
            Singleton<ClassDB>::instance()->registerClass(
                []() -> T * { return new T; }, T::className(), P::className());
            T::static_register();
        }
    };
} // namespace phi
