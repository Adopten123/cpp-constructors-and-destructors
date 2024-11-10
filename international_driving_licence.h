#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence {
public:
    //----------------------------------------------VTABLE--------------------------------------------------

    using PrintIDFunction = void(*)(const InternationalDrivingLicence* int_licence);
    using DeleteFunction = void(*)(InternationalDrivingLicence* int_licence);
    using GetIDFunction = int(*)(const InternationalDrivingLicence* id);

    struct Vtable {
        const PrintIDFunction PrintID;
        DeleteFunction Delete;
        const GetIDFunction GetID;
    };

    static void SetVtable(InternationalDrivingLicence* int_licence) {
        *(InternationalDrivingLicence::Vtable**)int_licence = &InternationalDrivingLicence::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const InternationalDrivingLicence::Vtable*)licence_.GetVtable();
    }

    Vtable* GetVtable() {
        return (InternationalDrivingLicence::Vtable*)licence_.GetVtable();
    }

    //----------------------------------------------VTABLE--------------------------------------------------
    InternationalDrivingLicence()
        : licence_()
    {
        InternationalDrivingLicence::SetVtable(this);
        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : licence_(other.licence_)
    {
        InternationalDrivingLicence::SetVtable(this);
        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
        DrivingLicence::SetVtable((DrivingLicence*)this);
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
    DrivingLicence licence_;

    static InternationalDrivingLicence::Vtable VTABLE;

    static void PrintID(const InternationalDrivingLicence* int_licence) {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << int_licence->licence_.GetID() << std::endl;
    }

    static void Delete(InternationalDrivingLicence* int_licence) {
        delete int_licence;
    }

    static int GetID(const InternationalDrivingLicence* licence) {
        return licence->licence_.GetID();
    }
};

InternationalDrivingLicence::Vtable InternationalDrivingLicence::VTABLE = { InternationalDrivingLicence::PrintID,
                                                                            InternationalDrivingLicence::Delete, InternationalDrivingLicence::GetID};