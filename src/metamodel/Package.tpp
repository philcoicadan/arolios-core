/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */



using namespace arolios;
using namespace arolios::omm;
        
        
        template <typename T1> void Package::insert (const std::shared_ptr<Packageable_element>& p_elem //,std::unordered_map<std::string,std::shared_ptr<T>>& p_map)
        ) {
            const std::shared_ptr<T1> elem_ptr = std::dynamic_pointer_cast<T1>(p_elem);
            const auto & pair = std::make_pair<const std::string&, const std::shared_ptr<T1>&>(p_elem->name(), elem_ptr);
            std::unordered_map<std::string,std::shared_ptr<T1>>& internal_map = element_type_map<T1>();
            internal_map.insert(pair);
            pk_elems_.insert (pair) ;
        }
        
        template <typename T1> void Package::rename (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) 
        {
            const std::shared_ptr<T1> elem_ptr = std::dynamic_pointer_cast<T1>(p_elem);
            const auto & pair = std::make_pair<const std::string&, const std::shared_ptr<T1>&>(p_new_name, elem_ptr);
            std::unordered_map<std::string,std::shared_ptr<T1>>& internal_map = element_type_map<T1>();

            internal_map.insert(pair);
            internal_map.erase(p_elem->name());
            pk_elems_.insert (pair) ;
            pk_elems_.erase(p_elem->name());

        }
        
        template <typename T1> void Package::erase (const std::shared_ptr<Packageable_element>& p_elem //,std::unordered_map<std::string,std::shared_ptr<T>>& p_map)
        ) {
            std::unordered_map<std::string,std::shared_ptr<T1>>& internal_map = element_type_map<T1>();
            auto search = internal_map.find(p_elem->name());
            if ( search != internal_map.end()) {
                internal_map.erase(search->first);
                pk_elems_.erase (search->first) ;
            } else {
                throw exception::Failure("Packageable_element " + p_elem->name() + " not found");
            }
        }
     