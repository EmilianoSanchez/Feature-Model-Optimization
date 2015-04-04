#ifndef PROPERTYMAP_H_INCLUDED
#define PROPERTYMAP_H_INCLUDED

#include <map>

/*! \class PropertyMap
 * \brief class used to manage a set of properties (attributes) referenced by a key value.
 * SoftElement and Feature inherit this class.
 */
template<typename Key, typename Value>
class PropertyMap{
private:
    std::map<Key,Value> attributes;
public:
    Value get_attribute(Key attribute){
        if(attributes.find(attribute)!=attributes.end())
            return attributes[attribute];
        else
            //In fact, it should throw an exception.
            return Value();
    };

    void set_attribute(Key attribute,Value value){
        attributes[attribute]=value;
    };

    const std::map<Key,Value> &get_attributes() const{
        return attributes;
    };
};

#endif
