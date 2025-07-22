#include "src/mesure/MesureRecordController.h"

MesureRecordController *MesureRecordController::s_instance = nullptr;

MesureRecordController *MesureRecordController::instance()
{
    if (!s_instance)
    {
        s_instance = new MesureRecordController();
    }
    return s_instance;
}

MesureRecordController::MesureRecordController (QObject *parent): QObject{parent}
{

}

MesureRecordController::~MesureRecordController()
{

}

