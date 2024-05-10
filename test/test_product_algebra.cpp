//#include "variable.h"
//#include "gtest/gtest.h"
//#include "set.h"
//#include "product_algebra.h"
//
//
//auto x = make_shared_continuous(std::make_shared<std::string>("x"));
//auto y = make_shared_continuous(std::make_shared<std::string>("y"));
//
//
//auto a = make_shared_symbolic(std::make_shared<std::string>("a"),
//        make_shared_all_elements(std::set<std::string>{"a", "b", "c"}));
//auto u = make_shared_symbolic(std::make_shared<std::string>("u"),
//                              make_shared_all_elements(std::set<std::string>{"u", "v", "w"}));
//
//
//TEST(ProductAlgebra, VariableGetting) {
//    auto map1 = std::make_shared<VariableMap>();
//    map1->insert({x, x->domain});
//    map1->insert({y, y->domain});
//    map1->insert({y, y->domain});
//    auto event1 = SimpleEvent(map1);
//
//    auto map2 = std::make_shared<VariableMap>();
//    map2->insert({x, x->domain});
//    map2->insert({a, a->domain});
//    auto event2 = SimpleEvent(map2);
//
//    ASSERT_TRUE(map1->size() == 2);
//
//    auto event2_variables = event2.get_variables();
//    ASSERT_TRUE(event2_variables.size() == 2);
//    auto all_variables = event2.merge_variables(event1.get_variables());
//    ASSERT_TRUE(all_variables.size() == 3);
//}
//
//TEST(ProductAlgebra, Intersection) {
//    auto variables1 = VariableSet({x, y});
//    auto variables2 = VariableSet({y, a});
//    auto event1 = make_shared_simple_event(variables1);
//    auto event2 = make_shared_simple_event(variables2);
//    auto intersection = std::static_pointer_cast<SimpleEvent>(event1->intersection_with(event2));
//    ASSERT_TRUE(intersection->variable_map->size() == 3);
//    ASSERT_FALSE(intersection->is_empty());
//}
//
//TEST(ProductAlgebra, Complement){
//    auto variables = VariableSet({x, a});
//    auto event = make_shared_simple_event(variables);
//    auto complement = event->complement();
//    ASSERT_TRUE(complement->empty());
//    (*event->variable_map)[a] = make_shared_set(make_shared_set_element("a", a->domain->all_elements), a->domain->all_elements);
//    (*event->variable_map)[x] = closed(0, 1);
//    complement = event->complement();
//    ASSERT_EQ(complement->size(), 2);
//}
//
//TEST(ProductAlgebra, SimplifyOnce){
//    auto variables = VariableSet({x, y});
//    auto event1 = make_shared_simple_event(variables);
//    (*event1->variable_map)[x] = closed(0, 1);
//    (*event1->variable_map)[y] = closed(0, 1);
//
//    auto event2 = make_shared_simple_event(variables);
//    (*event2->variable_map)[x] = closed(0, 1);
//    (*event2->variable_map)[y] = closed(1, 2);
//    auto composite_event = make_shared_event(event1);
//    composite_event->simple_sets->insert(event2);
//    auto [result, changed] = composite_event->simplify_once();
//    ASSERT_TRUE(changed);
//    ASSERT_EQ(result->simple_sets->size(), 1);
//
//    auto contained_event = std::static_pointer_cast<SimpleEvent>(*result->simple_sets->begin());
//    ASSERT_EQ(contained_event->variable_map->size(), 2);
//    ASSERT_EQ(contained_event->variable_map->at(x)->simple_sets->size(), 1);
//    ASSERT_EQ(*contained_event->variable_map->at(y), *closed(0, 2));
//
//    auto simplified = composite_event->simplify();
//    ASSERT_EQ(*simplified, *result);
//
//}