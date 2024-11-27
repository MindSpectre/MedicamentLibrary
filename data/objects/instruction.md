# Instruction to change data

<!-- TOC -->

* [Instruction to change data](#instruction-to-change-data)
    * [Add field for db entity](#add-field-for-db-entity)
    * [Add property to db entity](#add-property-to-db-entity)

<!-- TOC -->

## Add field for db entity

- Add field to entity in `data/objects/base/include`
- Define field name in `struct field_name : common_fields_names`
- Add converting to record

```C++
record.push_back(std::make_unique<common::database::Field<FieldType>>
    (field_name::type, type_));
```

- Add converting from record

```C++
// For regular record
if (field_name == field_name::type)
{
    type_ = field->as<FieldType>();
}
// For view record
if (field_name == field_name::type)
{
    type_ = viewed->extract(i);
}
```

- Change JSON serialization

```C++
result[field_name::type] = type_;
```

- Add getters/setters, change constructor/destructor if necessary

```C++
[[nodiscard]] const FieldType& get_type() const
{
    return type_;
}
void set_type(const FieldType& type)
{
    type_ = type;
}
```

- Go ahead with DAOs, follow up to special DAO and change `setup` function

```C++
const auto type_field = common::database::make_field_shared_by_default<FieldType>(
objects::YourObject::field_name::type);

value_fields_.push_back(type_field); // or other type
```

- Clear dependencies if compile errors occur

## Add property to db entity

This is a lot simpler than change fields, because it doesn't break already exist code

- Go to `data/objects/properties/YourObject/source/` and add to property definition filename of field
  kind of `static constexpr auto new_prop = "new";`
- Create a file in include properties directory with property class
- Include a file in `YourObject_properties.hpp` and link it in cmake
- Move to `PropertiesFactory` class and add a case for your new property

Remember to add testsuites for each property to ensure it works properly