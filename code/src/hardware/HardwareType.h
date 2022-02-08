#ifndef HARDWARETYPE_H
#define HARDWARETYPE_H

namespace HardwareType
{
    enum TypeList { OBSERVER, SERVER, TURRET };

    static bool isEqual(TypeList t1, TypeList t2) { return t1 == t2; }

    class HardwareTypeData
    {
	public:
	    HardwareTypeData(TypeList type) : _type(type) { }

	    TypeList getType() { return _type; }

	private:
	    TypeList _type;
    };
};

#endif
