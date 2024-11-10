#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
public:
    //----------------------------------------------VTABLE--------------------------------------------------

    using PrintIDFunction = void(*)(const DrivingLicence* licence);
    using DeleteFunction = void(*)(DrivingLicence* licence);
    using GetIDFunction = int(*)(const DrivingLicence* id);

    struct Vtable {
        const PrintIDFunction PrintID;
        DeleteFunction Delete;
        const GetIDFunction GetID;
    };

    static void SetVtable(DrivingLicence* licence) {
        *(DrivingLicence::Vtable**)licence = &DrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const DrivingLicence::Vtable*)id_.GetVtable();
    }

    Vtable* GetVtable() {
        return (DrivingLicence::Vtable*)id_.GetVtable();
    }

    //----------------------------------------------VTABLE--------------------------------------------------
    DrivingLicence() 
        : id_()
    {
        DrivingLicence::SetVtable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : id_(other.id_)
    {
        DrivingLicence::SetVtable(this);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVtable((IdentityDocument*)this);
    }

    operator IdentityDocument() const {
        return { id_ };
    }

    void PrintID() const {
        GetVtable()->PrintID(this);
    }

    void Delete() {
        GetVtable()->Delete(this);
    }

    int GetID() const {
        return GetVtable()->GetID(this);
    }

private:
    IdentityDocument id_;

    static DrivingLicence::Vtable VTABLE;

    static void PrintID(const DrivingLicence* licence) {
        std::cout << "DrivingLicence::PrintID() : "sv << licence->id_.GetID() << std::endl;
    }

    static int GetID(const DrivingLicence* licence) {
        return licence->id_.GetID();
    }

    static void Delete(DrivingLicence* licence) {
        delete licence;
    }
};

DrivingLicence::Vtable DrivingLicence::VTABLE = { DrivingLicence::PrintID, DrivingLicence::Delete, DrivingLicence::GetID };