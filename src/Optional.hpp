#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

template<typename T> struct Optional {
public:
    //default constructor, the object does not have any data, it does not exist
    Optional() {
        t_exists = false;
        t_value = T();
    }

    //allocate some memory for the value if it is not a null pointer
    Optional(T value) {
        t_exists = true;
        t_value = value;
    }

    ~Optional() {}

    //returns true if the value associated with this Optional is meant to exist
    bool exists() {
        return t_exists;
    }

    //not guaranteed to be useful if exists() returns false!
    T value() {
        return t_value;
    }

private:
    T t_value;
    bool t_exists;
};

#endif //OPTIONAL_HPP
