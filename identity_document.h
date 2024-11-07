#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
public:
    //----------------------------------------------VTABLE--------------------------------------------------

    using PrintIDFunction = void(*)(const IdentityDocument* id);
    using PrintUniqueIDCountFunction = void(*)();
    using DeleteFunction = void(*)(IdentityDocument* id);
    using GetIDFunction = int(*)(const IdentityDocument* id);

    struct Vtable {
        const PrintIDFunction PrintID;
        const PrintUniqueIDCountFunction PrintUniqueIDCount;
        DeleteFunction Delete;
        const GetIDFunction GetID;
    };

    static void SetVtable(IdentityDocument* id) {
        *(IdentityDocument::Vtable**)id = &IdentityDocument::VTABLE;
    }

    const IdentityDocument::Vtable* GetVtable() const {
        return vptr_;
    }

    IdentityDocument::Vtable* GetVtable() {
        return vptr_;
    }

    //----------------------------------------------VTABLE--------------------------------------------------

    //----------------------------------------------CONSTRUCTORS--------------------------------------------
    IdentityDocument()
        : unique_id_(++unique_id_count_) 
    {
        IdentityDocument::SetVtable(this);
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_)
    {
        IdentityDocument::SetVtable(this);
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    //----------------------------------------------CONSTRUCTORS--------------------------------------------

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
        vptr_->PrintID(this);
    }

    void Delete() {
        vptr_->Delete(this);
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    int GetID() const {
        return vptr_->GetID(this);
    }

private:
    IdentityDocument::Vtable* vptr_ = nullptr;
    static int unique_id_count_;
    int unique_id_;

    static IdentityDocument::Vtable VTABLE;

    static void PrintID(const IdentityDocument* id) {
        std::cout << "IdentityDocument::PrintID() : "sv << id->unique_id_ << std::endl;
    }

    static int GetID(const IdentityDocument* id) {
        return id->unique_id_;
    }

    static void Delete(IdentityDocument* id) {
        delete id;
    }


};

int IdentityDocument::unique_id_count_ = 0;

IdentityDocument::Vtable IdentityDocument::VTABLE = { IdentityDocument::PrintID, IdentityDocument::PrintUniqueIDCount, IdentityDocument::Delete, IdentityDocument::GetID };