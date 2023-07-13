#pragma once
#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>

template<typename T>
class ForwardList 
{
public:
    ForwardList() : head(nullptr) {}

    ~ForwardList() = default;

    void push_front(const T& value) 
    {
        std::unique_ptr<Node> newNode = std::make_unique<Node>(value);
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    void iterate() const 
    {
        Node* current = head.get();
        while (current != nullptr) 
        {
            std::cout << current->data << " ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }

    std::size_t size() const 
    {
        std::size_t count = 0;
        Node* current = head.get();
        while (current != nullptr) 
        {
            ++count;
            current = current->next.get();
        }
        return count;
    }

    void pop_front() 
    {
        if (head != nullptr) 
        {
            head = std::move(head->next);
        }
    }

    void clear() 
    {
        head = nullptr;
    }

    ForwardList(ForwardList&& other) noexcept = default;

    ForwardList& operator=(ForwardList&& other) noexcept = default;

    void push_front(T&& value) 
    {
        std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(value));
        newNode->next = std::move(head);
        head = std::move(newNode);
    }

    ForwardList(const std::initializer_list<T>& values) : head(nullptr) 
    {
        for (const T& value : values) 
        {
            push_front(value);
        }
    }

    ForwardList split_when(std::function<bool(const T&)> condition) 
    {
        ForwardList tail;
        if (head == nullptr) 
        {
            return tail;
        }

        Node* current = head.get();
        while (current->next != nullptr && !condition(current->next->data)) 
        {
            current = current->next.get();
        }

        if (current->next != nullptr) 
        {
            tail.head = std::move(current->next);
            current->next = nullptr;
        }

        return tail;
    }

private:
    struct Node
    {
        T data;
        std::unique_ptr<Node> next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> head;
};
