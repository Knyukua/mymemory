#pragma once

#include <type_traits>
#include <utility>

namespace my
{
    template<typename Type>
    struct default_delete
    {
        void operator()(Type* var)
        {
            delete var;
        }
    };

    template<typename Type>
    struct default_delete<Type[]>
    {
        void operator()(std::remove_extent_t<Type>* arr)
        {
            delete[] arr;
        }
    };

    template<typename Type, typename Deleter = default_delete<Type>>
    class unique_ptr
    {
        using ptr_t = Type*;
        using ref_t = Type&;
        using del_t = Deleter;

    public:
        unique_ptr(const unique_ptr& other) = delete;
        unique_ptr& operator=(const unique_ptr& other) = delete;

        explicit unique_ptr()
            : m_ptr(nullptr)
        {}

        explicit unique_ptr(ptr_t ptr)
            : m_ptr(ptr)
        {}

        explicit unique_ptr(ptr_t ptr, del_t deleter)
            : m_ptr(ptr)
            , m_del(deleter)
        {}

        ~unique_ptr()
        {
            if (m_ptr)
            {
                m_del(m_ptr);
            }
        }

        ptr_t get() const
        {
            return m_ptr;
        }

        operator bool() const
        {
            return m_ptr;
        }

        ptr_t operator->() const
        {
            return m_ptr;
        }

        ref_t operator*() const
        {
            return *m_ptr;
        }

    private:
        ptr_t m_ptr;
        del_t m_del;
    };

    template<typename Type, typename Deleter>
    class unique_ptr<Type[], Deleter>
    {
        using ptr_t = std::remove_extent_t<Type>*;
        using ref_t = std::remove_extent_t<Type>&;
        using del_t = Deleter;

    public:
        unique_ptr(const unique_ptr& other) = delete;
        unique_ptr& operator=(const unique_ptr& other) = delete;

        explicit unique_ptr()
            : m_ptr(nullptr)
        {}

        explicit unique_ptr(ptr_t ptr)
            : m_ptr(ptr)
        {}

        explicit unique_ptr(ptr_t ptr, del_t deleter)
            : m_ptr(ptr)
            , m_del(deleter)
        {}

        ~unique_ptr()
        {
            if (m_ptr)
            {
                m_del(m_ptr);
            }
        }

        ptr_t get() const
        {
            return m_ptr;
        }

        operator bool() const
        {
            return m_ptr;
        }

        ref_t operator[](int i) const
        {
            return m_ptr[i];
        }

    private:
        ptr_t m_ptr;
        del_t m_del;
    };

    template<typename Type, typename... Args>
    std::enable_if_t<!std::is_array_v<Type>, unique_ptr<Type, default_delete<Type>>>
    make_unique(Args&&... args)
    {
        return unique_ptr(new Type(std::forward<Args>(args)...));
    }

    template<typename Type>
    std::enable_if_t<std::is_array_v<Type> && std::extent_v<Type> == 0, unique_ptr<Type, default_delete<Type>>>
    make_unique(unsigned int size)
    {
        using BaseT = std::remove_extent_t<Type>;
        return unique_ptr<Type>(new BaseT[size]);
    }
}