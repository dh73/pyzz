#ifndef pyzz_netlist__H
#define pyzz_netlist__H

#include "pyzzwrapper.h"

namespace pyzz
{

class Netlist :
    public type_base_with_new<Netlist>
{
public:

    Netlist(bool empty=false);
    ~Netlist();

    static void initialize(PyObject* module);

    static ref<PyObject> read_aiger(PyObject* o);
    static ref<PyObject> read(PyObject* o);

    void write_aiger(PyObject* args);
    void write(PyObject* args);

    ref<PyObject> get_True();

    ref<PyObject> add_PI(PyObject* args);
    ref<PyObject> add_PO(PyObject* args, PyObject* kwds);
    ref<PyObject> add_Flop(PyObject* args, PyObject* kwds);

    void add_property(PyObject* o);
    void add_constraint(PyObject* o);
    void add_fair_property(PyObject* o);
    void add_fair_constraint(PyObject* o);

    ref<PyObject> n_PIs();
    ref<PyObject> n_POs();
    ref<PyObject> n_Flops();
    ref<PyObject> n_Ands();

    ref<PyObject> n_properties();
    ref<PyObject> n_constraints();
    ref<PyObject> n_fair_properties();
    ref<PyObject> n_fair_constraints();

    ref<PyObject> get_PIs();
    ref<PyObject> get_POs();
    ref<PyObject> get_Flops();
    ref<PyObject> get_Ands();

    ref<PyObject> get_properties();
    ref<PyObject> get_constraints();
    ref<PyObject> get_fair_properties();
    ref<PyObject> get_fair_constraints();

    ref<PyObject> get_flop_init();
    ref<PyObject> get_names();

    ref<PyObject> copy();

    ref<PyObject> uporder(PyObject* args);

    void remove_unreach();

public:

    ZZ::Netlist N;

private:

    void assure_pobs();
};

} // namesapce pyzz

#endif // #ifndef pyzz_netlist__H
