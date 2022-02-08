#ifndef CONTROLMODULE_H
#define CONTROLMODULE_H

//based by joystick
class ControlModule
{
    public:
	virtual void inputAxisX(int input) = 0;

	virtual void inputAxisY(int input) = 0;

	virtual void pushButton(int time) = 0;
};

#endif
