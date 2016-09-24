#ifndef PROBABLE_H
#define PROBABLE_H

using namespace std;

//Simple containers for values and their probability.
template<typename T> struct Probable {
    T value;
    int p;

    Probable(T value, int probability) {
        this.value = value;
        this.probability = probability;
    }

    //equality is based on the value itself rather than the probability
    bool operator==(const Probable& other) const {
        return value == other.value;
    }
}

#endif //PROBABLE_H
