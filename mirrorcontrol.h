#ifndef MIRRORCONTROL_H
#define MIRRORCONTROL_H

#include "MTIDefinitions.h"
#include "MTIDevice.h"
#include "MTIDataGenerator.h"

class MirrorControl
{

private:
    MTIDevice theMirror;
public:
    MirrorControl();
    bool Connection();
};

#endif // MIRRORCONTROL_H
