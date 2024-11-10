#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

class Passport {
public:
    //----------------------------------------------VTABLE--------------------------------------------------
    
    using PrintIDFunction = void(*)(const Passport* passport);
    using DeleteFunction = void(*)(Passport* passport);
    using GetIDFunction = int(*)(const Passport* id);

    struct Vtable {
        const PrintIDFunction PrintID;
        DeleteFunction Delete;
        const GetIDFunction GetID;
    };

    static void SetVtable(Passport* passport) {
        *(Passport::Vtable**)passport = &Passport::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const Passport::Vtable*)id_.GetVtable();
    }

    Vtable* GetVtable() {
        return (Passport::Vtable*)id_.GetVtable();
    }

    //----------------------------------------------VTABLE--------------------------------------------------
    Passport()
        : id_()
        , expiration_date_(GetExpirationDate()) 
    {
        Passport::SetVtable(this);
        std::cout << "Passport::Ctor()"sv << std::endl;
    }

    Passport(const Passport& other)
        : id_(other.id_)
        , expiration_date_(other.expiration_date_) 
    {
        Passport::SetVtable(this);
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    ~Passport() {
        std::cout << "Passport::Dtor()"sv << std::endl;
        IdentityDocument::SetVtable((IdentityDocument*)this);
    }

    operator IdentityDocument() const {
        id_.SetVtable((IdentityDocument*)this);
        return id_;
    }

    void PrintID() const {
        GetVtable()->PrintID(this);
    }

    void PrintUniqueIDCount() const {
        id_.PrintUniqueIDCount();
    }

    void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << id_.GetID() << std::endl;
    }

    void Delete() {
        GetVtable()->Delete(this);
    }

    int GetID() const {
        return GetVtable()->GetID(this);
    }

private:
    IdentityDocument id_;
    const struct tm expiration_date_;

    tm GetExpirationDate() {
	    time_t t = time(nullptr);
	    tm exp_date = *localtime(&t);
	    exp_date.tm_year += 10;
	    mktime(&exp_date);
	    return exp_date;
	}

    static Passport::Vtable VTABLE;

    static void PrintID(const Passport* passport) {
        std::cout << "Passport::PrintID() : "sv << passport->id_.GetID();
        std::cout << " expiration date : "sv << passport->expiration_date_.tm_mday << "/"sv << passport->expiration_date_.tm_mon << "/"sv
            << passport->expiration_date_.tm_year + 1900 << std::endl;
    }

    static int GetID(const Passport* passport) {
        return passport->id_.GetID();
    }

    static void Delete(Passport* passport) {
        delete passport;
    }

};

Passport::Vtable Passport::VTABLE = { Passport::PrintID,Passport::Delete, Passport::GetID};