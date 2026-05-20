// =================================
// Copyright (c) 2026 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.json;

import std;
import util.code.formatter;

export namespace util {

export namespace json {}

enum class JsonValueType : int
{
    none = 0,
    object = 1 << 0,
    array = 1 << 1,
    string = 1 << 2,
    number = 1 << 3,
    boolean = 1 << 4,
    null = 1 << 5,
    any = object | array | string | number | boolean | null
};

constexpr JsonValueType operator|(JsonValueType left, JsonValueType right) noexcept
{
    return JsonValueType(int(left) | int(right));
}

constexpr JsonValueType operator&(JsonValueType left, JsonValueType right) noexcept
{
    return JsonValueType(int(left) & int(right));
}

constexpr JsonValueType operator~(JsonValueType type) noexcept
{
    return JsonValueType(~int(type));
}

std::string JsonValueTypeStr(JsonValueType type);

class JsonValue
{
public:
    JsonValue(JsonValueType type_) noexcept;
    JsonValue(const JsonValue&) = delete;
    JsonValue& operator=(const JsonValue&) = delete;
    virtual ~JsonValue();
    virtual JsonValue* Clone() const = 0;
    inline JsonValueType Type() const noexcept { return type; }
    inline bool IsObject() const noexcept { return type == JsonValueType::object; }
    inline bool IsArray() const noexcept { return type == JsonValueType::array; }
    inline bool IsStructuredValue() const noexcept { return IsObject() || IsArray(); }
    inline bool IsString() const noexcept { return type == JsonValueType::string; }
    inline bool IsNumber() const noexcept { return type == JsonValueType::number; }
    inline bool IsBoolean() const noexcept { return type == JsonValueType::boolean; }
    inline bool IsNull() const noexcept { return type == JsonValueType::null; }
    virtual std::string ToString() const = 0;
    virtual void Write(CodeFormatter& formatter);
private:
    JsonValueType type;
};

class JsonString : public JsonValue
{
public:
    JsonString();
    JsonString(const std::u32string& value_);
    void Append(char32_t c);
    JsonValue* Clone() const override;
    inline const std::u32string& Value() const noexcept { return value; }
    void SetValue(const std::u32string& value_);
    std::u16string JsonCharStr(char32_t c) const;
    std::string ToString() const override;
private:
    std::u32string value;
};

class JsonNumber : public JsonValue
{
public:
    JsonNumber() noexcept;
    JsonNumber(double value_) noexcept;
    JsonValue* Clone() const override;
    inline double Value() const noexcept { return value; }
    std::string ToString() const override;
private:
    double value;
};

class JsonBool : public JsonValue
{
public:
    JsonBool() noexcept;
    JsonBool(bool value_) noexcept;
    JsonValue* Clone() const override;
    inline bool Value() const noexcept { return value; }
    std::string ToString() const override;
private:
    bool value;
};

class JsonArray;

class JsonObject : public JsonValue
{
public:
    JsonObject() noexcept;
    void AddField(const std::u32string& fieldName, std::unique_ptr<JsonValue>&& fieldValue);
    inline int FieldCount() const noexcept { return static_cast<int>(fieldValues.size()); }
    JsonValue* GetField(const std::u32string& fieldName) const noexcept;
    bool HasField(const std::u32string& fieldName) const noexcept;
    JsonString* GetStringField(const std::u32string& fieldName) const;
    JsonNumber* GetNumberField(const std::u32string& fieldName) const;
    JsonBool* GetBooleanField(const std::u32string& fieldName) const;
    JsonObject* GetObjectField(const std::u32string& fieldName) const;
    JsonArray* GetArrayField(const std::u32string& fieldName) const;
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> fieldValues;
    std::map<std::u32string, JsonValue*> fieldMap;
};

class JsonArray : public JsonValue
{
public:
    JsonArray() noexcept;
    void AddItem(std::unique_ptr<JsonValue>&& item);
    inline int Count() const noexcept { return static_cast<int>(items.size()); }
    JsonValue* GetItem(int index) const;
    JsonValue* operator[](int index) const;
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> items;
};

class JsonNull : public JsonValue
{
public:
    JsonNull() noexcept;
    JsonValue* Clone() const override;
    std::string ToString() const override;
};

} // namespace util
