#pragma once

#include <unordered_set>
#include <vector>


template<typename T>
using SimpleSetType = std::unordered_set<T>;

/**
* Interface class for simple sets.
*/
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class SimpleSetWrapper {
public:
    /**
     * @return The simple set pointer that implements the interface.
     */
    T_SimpleSet *get_simple_set() const {
        auto *derived = static_cast<const T_SimpleSet *>(this);
        return (T_SimpleSet *) derived;
    }

    /**
    * Intersect this with another simple set.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param other the other simples set.
    * @return The intersection of both as simple set.
    */
    [[nodiscard]] T_SimpleSet intersection_with(const T_SimpleSet &other) const {
        return get_simple_set()->simple_set_intersection_with(other);
    }

    /**
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @return The complement of this simple set as disjoint composite set.
    */
    T_CompositeSet complement() const {
        return get_simple_set()->simple_set_complement();
    }

    /**
    * Check if an elementary event is contained in this.
    *
    * This method depends on the type of simple set and has to be overwritten.
    *
    * @param element The element to check.
    * @return True if the element is contained in this.
    */
    [[nodiscard]] bool contains(const T_Elementary &element) const {
        return get_simple_set()->simple_set_contains(element);
    }

    /**
     * Check if another simple set is contained in this.
     *
     * @param other The other simple set to check.
     * @return True if the other simple set is contained in this.
     */
    bool contains(const T_SimpleSet &other) const {
        return !intersection_with(other).is_empty();
    }

    /**
     * This method depends on the type of simple set and has to be overwritten.
     *
     * @return True if this is empty.
     */
    [[nodiscard]] bool is_empty() const {
        return get_simple_set()->simple_set_is_empty();
    }

    /**
     * Form the difference with another simple set.
     * @param other The other simple set.
     * @return The difference as disjoint composite set.
     */
    [[nodiscard]] T_CompositeSet difference_with(const T_SimpleSet &other) const {

        // get the intersection of both atomic simple_sets
        T_SimpleSet intersection = intersection_with(other);

        // if the intersection is empty, return the current atomic interval as interval
        if (intersection.is_empty()) {
            return T_CompositeSet({*get_simple_set()});
        }

        // get the complement of the intersection
        T_CompositeSet complement_of_intersection = intersection.complement();

        // initialize the difference vector
        std::unordered_set<T_SimpleSet> difference;

        // for every interval in the complement of the intersection
        for (const T_SimpleSet &simple_set: complement_of_intersection.simple_sets) {

            // intersect this with the current complement of the intersection
            T_SimpleSet intersection_with_complement = intersection_with(simple_set);

            // if the intersection with the complement is not empty, append it to the difference vector
            if (!intersection_with_complement.is_empty()) {
                difference.insert(intersection_with_complement);
            }
        }

        return T_CompositeSet(difference);
    }

};


/**
* Interface class for composite elements.
* */
template<typename T_CompositeSet, typename T_SimpleSet, typename T_Elementary>
class CompositeSetWrapper {
public:

    /**
     * @return The composite set pointer that implements the interface.
     */
    T_CompositeSet *get_composite_set() const {
        auto *derived = static_cast<const T_CompositeSet *>(this);
        return (T_CompositeSet *) derived;
    }



    /**
     * Default Constructor.
     */
    CompositeSetWrapper() {
        simple_sets = SimpleSetType<T_SimpleSet>();
    };

    /**
     * Construct a composite set from a unordered set of simple sets.
     */
    explicit CompositeSetWrapper(const SimpleSetType<T_SimpleSet> &simple_sets_) {
        for (auto simple_set: simple_sets) {
            if (!simple_set.is_empty()) {
                simple_sets.push_back(simple_set);
            }
        }
    }

    /**
     * @return True if this is empty.
     */
    [[nodiscard]] bool is_empty() const {
        return simple_sets.empty();
    }

    /**
     * @return True if the composite set is disjoint union of simple sets.
     */
    [[nodiscard]] bool is_disjoint() const {
        for (const auto &[first, second]: unique_combinations(simple_sets_as_vector())) {
            if (!first.intersection_with(second).is_empty()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Simplify the composite set into a shorter but equal representation.
     * The size (shortness9 refers to the number of simple sets contained.
     *
     * This method depends on the type of simple set and has to be overloaded.
     *
     * @return The simplified composite set into a shorter but equal representation.
     */
    T_CompositeSet simplify() {
        return get_composite_set()->composite_set_simplify();
    }

    bool operator==(const T_CompositeSet &other) const {
        return simple_sets == other.simple_sets;
    }

    /**
     * @return the simple sets as vector.
     */
    std::vector<T_SimpleSet> simple_sets_as_vector() const {
        return std::vector<T_SimpleSet>(simple_sets.begin(), simple_sets.end());
    }

    /**
     * @return A string representation of this.
     */
    [[nodiscard]] std::string to_string() const {
        if (is_empty()) {
            return "∅";
        }
        std::string result;

        auto simple_sets_vector = simple_sets_as_vector();

        for (size_t i = 0; i < simple_sets_vector.size(); ++i) {
            result.append(simple_sets_vector[i].to_string());
            if (i != simple_sets_vector.size() - 1) {
                result.append(" u ");
            }
        }
        return result;
    }

//    /**
//     * Create an equal composite  set that contains a disjoint union of simple sets.
//     * @return The disjoint composite set.
//     */
//    T_CompositeSet make_disjoint() {
//        // initialize disjoint, non-disjoint and current sets
//        T_CompositeSet disjoint;
//        T_CompositeSet intersections;
//        T_CompositeSet current_disjoint;
//
//        // start with the initial split
//        std::tie(disjoint, intersections) = split_into_disjoint_and_non_disjoint();
//
//        // as long the splitting still produces non-disjoint sets
//        while (!intersections.is_empty()) {
//
//            // split into disjoint and non-disjoint sets
//            std::tie(current_disjoint, intersections) = intersections.split_into_disjoint_and_non_disjoint();
//
//            // extend the result by the disjoint sets
//            extend_vector(disjoint.simple_sets, current_disjoint.simple_sets);
//        }
//
////        disjoint.simple_sets.erase(unique(disjoint.simple_sets.begin(), disjoint.simple_sets.end()),
////                                   disjoint.simple_sets.end());
//        // simplify and return the disjoint set
//        return disjoint.simplify();
//    }

public:
    SimpleSetType<T_SimpleSet> simple_sets;
};


/**
 * Unique Combinations of elements within a vector.
 * The unique combinations are pairs of elements which exclude:
 * - symmetric pairs (A, A)
 * - (A,B) if (B, A) is already visited.
 *
 * @param elements The vector.
 * @return The unique combinations of elements of the vector.
 */
template<typename T>
std::vector<std::tuple<T, T>> unique_combinations(const std::vector<T> &elements) {

    // initialize result
    std::vector<std::tuple<T, T>> combinations;

    // for every pair of elements
    for (std::size_t i = 0; i < elements.size(); ++i) {

        // get element from first vector
        T current_element1 = elements[i];
        for (std::size_t j = 0; j < i; ++j) {
            T current_element2 = elements[j];
            std::tuple<T, T> combination = std::make_tuple(current_element1, current_element2);
            combinations.push_back(combination);
        }
    }
    return combinations;
}