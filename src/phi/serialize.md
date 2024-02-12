# Serialize

## Variant

| meaning | size / _byte_ |
| ------- | ------------- |
| type    | 1             |
| content | n             |

## integer

| meaning | size / _byte_     |
| ------- | ----------------- |
| content | sizeof(integer) 8 |

## real

| meaning | size / _byte_   |
| ------- | --------------- |
| content | sizeof(real) 16 |

## bool

| meaning | size / _byte_  |
| ------- | -------------- |
| content | sizeof(bool) 1 |

## string

(length, content, content, ...)
| meaning | size / _byte_ |
| ------- | ---------------------- |
| length | sizeof(uinteger) 8 |
| content | length \* sizeof(char) |

## vector<T>

(size, element, element, ...)
| meaning | size / _byte_ |
| ------- | ------------------------- |
| length | sizeof(uinteger) 8 |
| content | sum(sizeof(each_element)) |

## map|unordered_map<K, V>

(size, key, value, key, ...)
| meaning | size / _byte_ |
| ------- | ------------------------------------------ |
| length | sizeof(uinteger) 8 |
| content | sum(sizeof(each_key) + sizeof(each_value)) |

## Object

(className, properties)
| meaning | size / _byte_ |
| ---------- | ----------------- |
| className | real_sizeof(string) |
| properties | real_sizeof(map<string, Ref<Variant>>) |

## Builtin Function

**_unserializable_**

## Phi Function

(01, method, binds, properties)
| meaning | size / _byte_ |
| ---------- | ----------------- |
| flag | 1 |
| method | real_sizeof(method) |
| binds | real_sizeof(map<string, Ref<Variant>>) |
| properties | real_sizeof(map<uinteger, Ref<Variant>>) |

## Method

(state, binds)
| meaning | size / _byte_ |
| ---------- | ----------------- |
| state | real_sizeof(state) |
| binds | real_sizeof(vector<Ref<string>>) |

## State

(GCP, codes, labels, lines, chunk)
| meaning | size / _byte_ |
| ---------- | ----------------- |
| GCP | real_sizeof(State::gcp_t) |
| codes | real_sizeof(State::CodeSeq) |
| labels | real_sizeof(unordered_map<arg_t, arg_t>) |
| lines | real_sizeof(vector<arg_t>) |
| chunk | real_sizeof(string) |
