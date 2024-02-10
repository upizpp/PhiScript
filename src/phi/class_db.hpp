#pragma once

#include <functional>
#include <phi/exception.hpp>
#include <phi/flag.hpp>
#include <phi/singleton.hpp>
#include <phi/traits.hpp>
#include <phi/typedef.hpp>
#include <phi/variant.hpp>

#define METHOD_R(name, rename) Singleton<ClassDB>::instance()->registerMethod(#rename, &CLASS::name);
#define METHOD(name) METHOD_R(name, name)
#define PROPERTY_OFFSET(class, name) size_t(&((class *)0)->name)
#define CLASS
#define PROPERTY_R(name, rename) Singleton<ClassDB>::instance()->registerProperty<decltype(((CLASS *)0)->name)>(#rename, PROPERTY_OFFSET(CLASS, name))
#define PROPERTY(name) PROPERTY_R(name, name)
#define PhiClass(class_name, parent_class)         \
public:                                            \
	static void static_register();                 \
	static string className()                      \
	{                                              \
		return #class_name;                        \
	}                                              \
	static string parentClassName()                \
	{                                              \
		return #parent_class;                      \
	}                                              \
	virtual string getClass() const override       \
	{                                              \
		return #class_name;                        \
	}                                              \
	virtual string getParentClass() const override \
	{                                              \
		return #parent_class;                      \
	}                                              \
                                                   \
private:                                           \
	ClassRegister<class_name, parent_class> _;

namespace phi
{
	using callable_t = std::function<Ref<Variant>(const array &)>;

	struct ClassInfo
	{
		using type = Variant;
		using arg_list = vector<type>;
		using method = std::function<type(Object *, const array &)>;
		using initializer_t = std::function<Object *(void)>;

		ClassInfo *parent;
		map<string, method> methods;
		initializer_t initializer;
		map<string, std::pair<std::function<type(Object *)>, std::function<void(Object *, const type &)>>> properties;

		ClassInfo() {}
		ClassInfo(ClassInfo *p, const initializer_t &init) : parent(p), initializer(init) {}

		type call(Object *, const string &, const array &);

		bool hasProperty(const string &);
		bool hasMethod(const string &);
		method &getMethod(const string &);
	};

	class ClassDB
	{
	public:
		using type = Variant;
		using arg_list = vector<type>;

	private:
		map<string, ClassInfo> _M_classes;
		string _M_bound;
		string _M_calling;

	public:
		void bind(const string &class_name) { _M_bound = class_name; }
		void registerClass(const ClassInfo::initializer_t &initializer, const string &class_name, const string &parent)
		{
			ClassInfo *parent_info = _M_classes.find(parent) != _M_classes.end() ? &_M_classes[parent] : nullptr;
			_M_classes.insert({class_name, ClassInfo{parent_info, initializer}});
			bind(class_name);
		}

		template <typename F>
		void registerMethod(const string &name, const F &func)
		{
			auto caller = [=](Object *obj, const array &args) -> type
			{
				using seq = gen_index_seq_t<function_traits<F>::arity>;
				return class_call_impl<seq, F>::call(func, obj, args);
			};
			getInfo().methods.insert({name, caller});
		}
		template <typename T>
		void registerProperty(const string &name, size_t offset)
		{
			auto setter = [=](Object *obj, const type &value) -> void
			{
				char *addr = (char *)obj + offset;
				*reinterpret_cast<T *>(addr) = static_cast<T>(value);
			};
			auto getter = [=](Object *obj) -> type
			{
				char *addr = (char *)obj + offset;
				return type{*reinterpret_cast<T *>(addr)};
			};
			getInfo().properties.insert({name, {getter, setter}});
		}

	private:
		struct ArgumentHandler
		{
			size_t requiredCount;

			template <typename T>
			typename std::remove_cv<typename std::remove_reference<T>::type>::type handle(const array &args, size_t index)
			{
				return handleImpl<typename std::remove_cv<typename std::remove_reference<T>::type>::type>(args, index);
			}

		private:
			template <typename T>
			T handleImpl(const array &args, size_t index)
			{
				if (index >= args.size())
					throw ArgumentException(requiredCount, args.size(), Singleton<ClassDB>::instance()->_M_calling);
				return args[index]->convertTo(VariantType<T>::value);
			}
		};

		friend struct ArgumentHandler;
		friend struct class_call_impl;

		template <typename I, typename F, typename Enabled = void>
		struct class_call_impl;

		template <size_t... I, typename F>
		struct class_call_impl<index_seq<I...>, F,
							   typename std::enable_if<std::is_same<typename function_traits<F>::return_type, void>::value>::type>
		{
			static type call(F f, Object *obj, const array &args)
			{
				ArgumentHandler handler;
				handler.requiredCount = sizeof...(I);
				using class_type = typename function_traits<F>::class_type;
				(static_cast<class_type *>(obj)->*f)(handler.handle<typename function_traits<F>::template args<I>::type>(args, I)...);
				return *Variant::Null;
			}
		};

		template <size_t... I, typename F>
		struct class_call_impl<index_seq<I...>, F,
							   typename std::enable_if<!std::is_same<typename function_traits<F>::return_type, void>::value>::type>
		{
			static type call(F f, Object *obj, const array &args)
			{
				ArgumentHandler handler;
				handler.requiredCount = sizeof...(I);
				using class_type = typename function_traits<F>::class_type;
				return (static_cast<class_type *>(obj)->*f)(handler.handle<typename function_traits<F>::template args<I>::type>(args, I)...);
			}
		};

		friend struct call_impl;

		template <typename I, typename F, typename Enabled = void>
		struct call_impl;

		template <size_t... I, typename F>
		struct call_impl<index_seq<I...>, F,
						 typename std::enable_if<std::is_same<typename function_traits<F>::return_type, void>::value>::type>
		{
			static Ref<Variant> call(F f, const array &args)
			{
				ArgumentHandler handler;
				handler.requiredCount = sizeof...(I);
				f(handler.handle<typename function_traits<F>::template args<I>::type>(args, I)...);
				return Variant::Null;
			}
		};

		template <size_t... I, typename F>
		struct call_impl<index_seq<I...>, F,
						 typename std::enable_if<!std::is_same<typename function_traits<F>::return_type, void>::value>::type>
		{
			static Ref<Variant> call(F f, const array &args)
			{
				ArgumentHandler handler;
				handler.requiredCount = sizeof...(I);
				return f(handler.handle<typename function_traits<F>::template args<I>::type>(args, I)...);
			}
		};

	public:
		template <typename F>
		static callable_t toCallable(const F &func)
		{
			return [=](const array &args) -> Ref<Variant>
			{
				using seq = gen_index_seq_t<function_traits<F>::arity>;
				return call_impl<seq, F>::call(func, args);
			};
		}
		template <typename F>
		static callable_t toCallable(Object *obj, const F &func)
		{
			return [=](const array &args) -> Ref<Variant>
			{
				using seq = gen_index_seq_t<function_traits<F>::arity>;
				return class_call_impl<seq, F>::call(func, obj, args);
			};
		}
		static callable_t toCallable(Object *obj, const string &func)
		{
			return [=](const array &args) -> Ref<Variant>
			{
				return new Variant{Singleton<ClassDB>::instance()->call(obj, func, args)};
			};
		}

		ClassInfo &getInfo() { return _M_classes[_M_bound]; }

		type call(Object *obj, const string &method_name, const array &args = {});
		void set(Object *obj, const string &property_name, const type &value);
		type get(Object *obj, const string &property_name);
		bool hasProperty(const Object *obj, const string &property_name);
		bool hasMethod(const Object *obj, const string &method_name);

		ClassInfo &parent(const string &class_name) { return *_M_classes[class_name].parent; }
	};

	template <>
	RestParameters ClassDB::ArgumentHandler::handleImpl<RestParameters>(const array &args, size_t index);
	template <>
	Ref<Variant> ClassDB::ArgumentHandler::handleImpl<Ref<Variant>>(const array &args, size_t index);

	template <typename T, typename P>
	struct ClassRegister
	{
		ClassRegister()
		{
			Singleton<ClassDB>::instance()->registerClass([]() -> T *
														  { return new T; },
														  T::className(), P::className());
			T::static_register();
		}
	};
} // namespace phi
