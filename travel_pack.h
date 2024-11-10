#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack {
public:
    using PrintIDFunction = void(*)(const TravelPack* travel_pack);
    using DeleteFunction = void(*)(TravelPack* travel_pack);

    struct Vtable {
        const PrintIDFunction PrintID;
        DeleteFunction Delete;
    };

    static void SetVtable(TravelPack* travel_pack) {
        *(TravelPack::Vtable**)travel_pack = &TravelPack::VTABLE;
    }

    const Vtable* GetVtable() const {
        return (const TravelPack::Vtable*)id_.GetVtable();
    }

    Vtable* GetVtable() {
        return (TravelPack::Vtable*)id_.GetVtable();
    }

    TravelPack()
        : id_()
        , identity_doc1_(reinterpret_cast<IdentityDocument*>(new Passport()))
        , identity_doc2_(reinterpret_cast<IdentityDocument*>(new DrivingLicence()))
    {
        TravelPack::SetVtable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : id_(other.id_)
        , identity_doc1_(reinterpret_cast<IdentityDocument*>(new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_))))
        , identity_doc2_(reinterpret_cast<IdentityDocument*>(new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_))))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)
    {
        TravelPack::SetVtable(this);
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        delete identity_doc1_;
        delete identity_doc2_;
        std::cout << "TravelPack::Dtor()"sv << std::endl;
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

private:
    IdentityDocument id_;
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;

    static TravelPack::Vtable VTABLE;

    static void PrintID(const TravelPack* travel_pack) {
        travel_pack->identity_doc1_->PrintID();
        travel_pack->identity_doc2_->PrintID();
        travel_pack->additional_pass_.PrintID();
        travel_pack->additional_dr_licence_.PrintID();
    }

    static void Delete(TravelPack* travel_pack) {
        delete travel_pack;
    }
};

TravelPack::Vtable TravelPack::VTABLE = { TravelPack::PrintID, TravelPack::Delete };