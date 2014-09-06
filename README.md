# pyzz

`pyzz` is a python interface for [ABC/ZZ](https://bitbucket.org/niklaseen/abc-zz)

# Usage

First import the pyzz module

    import pyzz

The main classes in the module are:

* Netlist - an AIG.
* Wire - a (possibly) complemented AIG node.
* Solver - a SAT solver and a CNF generator combined in one
* Unroll - unrolls a sequential AIG
* WWMap - maps Wire objects to Wires, handling complemented Wire objects correctly.

## Netlist

The `netlist` class represents an AIG. It can be created from an AIGER file

    N = pyzz.netlist.read_aiger('some_aiger_file.aig')

created from scratch

    N = pyzz.netlist()

or copied from a previously created Netlist

    M, xlat = N.copy()

the mapping between the old and the new netlists is stored in `xlat`

The netlist can be saved back into a file

    N.write_aiger('some_new_aiger_file.aig')    

These methods create new wires

* `N.add_PI()` returns a new PI
* `N.add_PO(fanin=None)` returns a new PO. If `fanin` is specified, it is used as the fanin or the PO
* `N.add_Flop(init=None)` returns a new Flop. If `init` is specified it is used as the initial value of the Flop

The constant true wire is returned by calling

    N.get_True()

These methods return information about the AIG

* `N.n_PIs()`, `N.n_POs()`, `N.n_Flops()`, `N.n_Ands()`: return the number of PIs, POs, Flops, and And gates, respectively
* `N.get_PIs()`, `N.get_POs()`, `N.get_Flops()`, `N.get_Ands()`: return an iterator over the PIs, POs, Flops, and And gates, respectively

Properties are constrains are (possibly complemented) wires.

The properties and constraints can be accessed by

* `N.n_properties()`, `N.get_properties()`: returns an iterator over the properties
* `N.n_constrains()`, `N.get_constraints()`: returns an iterator over the constrains

The properties and constraints can be modified by

* `N.clear_properties()`, `N.clear_constraints()`
* `N.add_property()`, `N.add_property()`

## Wire

A wire represents a, possibly complemented, AIG node.

* `~w`: returns the complement of a wire
* `+w`: returns the non-complemented version of a wire
* `w.sign()`: returns whether the wire is complemented
* `w1 & w2`: returns the AND of two wires
* `w1 | w2`: returns the AND of two wires
* `w1 ^ w2`: returns the AND of two wires
* `w1.implies(w2)`: returns a wire that is true if `w1` implies `w2`
* `w1.equals(w2)`: returns a wire that is true if `w1` equals `w2`
* `w_if.ite(w_then, w_else)`: returns the ITE of the three wires

A wire can be queries to see what type of node it represents

* `w.is_PI()`: returns true if the wire is a PI
* `w.is_PO()`: returns true if the wire is a PO
* `w.is_Flop()`: returns true if the wire is a Flop
* `w.is_And()`: returns true if the wire is an AND gate

If the wire is an AND gate, accessing its fanins is done by

* `w[0]`: returns the left fanin
* `w[1]`: returns the right fanin

I the wire is a PO, its fanin in can be accessed using

* `w[0]`: returns the fanin of the PO
* `w[0]=u`: sets the fanin of the PO

If the wire is a Flop, its next-state function can accessed using

* `w[0]`: returns the next-state function of the Flop
* `w[0]=u`: sets the next-state function of the Flop

## Solver

A Solver object combines a SAT solver and a CNF generator. It can handle wires directly--the cone of the wires is then converted into CNF.

A solver is construct over a netlist:

    S = pyzz.solver(N)

A SAT query is done by using the `solve` method

    res = S.solve()

the `solve` method can be given assumptions

    res = S.solve( wassumption1, wassumption2, ... )

The result of the `solve` method can be one of 

* `pyzz.netlist.UNSAT`: the query was UNSAT
* `pyzz.netlist.SAT`: the query was SAT
* `pyzz.netlist.UNKNOWN`: the query was terminated due to timeout
* `pyzz.netlist.ERROR`: the query was terminated due to error

If the result is SAT, the values of the CEX can be queries by

    v = S[w]

The result of the result can be one of 

* `pyzz.netlist.l_True`: the value of `w` in the satisfying assignment is true
* `pyzz.netlist.l_False`: the value of `w` in the satisfying assignment is false
* `pyzz.netlist.l_Unknown`: the value of `w` in the satisfying assignment is not specified (e.g. the wire was not in the cone of the query)

## Unroll

The Unroll object unrolls a netlist in time.

The unroll object is created over a netlist

    U = pyzz.unroll(N)

The unroll object has two data members

* `U.N`: the netlist to be unrolled
* `U.F`: the unrolled netlist (F stands for frames)

To unroll a wire use

    fw = U[w,i]

`fw` is a wire in `U.F` that represents `w` at time `i`. If `w` is a sequence of wires, then `fw` is a sequence of wires.

When a wire is unrolled, all of its cone is unrolled as required to represent `w` at time `i`.

To check if a wire is already unrolled to a specific time 

    is_unrolled = (w,i) in U:

The number of unrolled frames (for the wire that is unrolled the most) can be checked using

    len(U)

## Utilities

Boolean operation over a more than two wire:

* `pyzz.conjunction(N, wires)`: returns the conjunction of all the wires in `wires`
* `pyzz.disjunction(N, wires)`: returns the conjunction of all the wires in `wires`
* `pyzz.equals(N, wires1, wires2)`: retuns a wire that is true if the values of the wires in `wires1` equals the respective values in `wires2`

# Examples

    from pyzz import *
    
    N = netlist() # construct a netlist
    
    w1 = N.add_PI() # create a new PI
    w2 = N.add_PI() # create a new PI

    po1 = N.add_PO() # create a new PO
    po1[0] = w1 & w2 # set the fanin of the PO to w1&w2

    po2 = N.add_PO(fanin=w1&w2) # similar to above, but sets the fanin during construction

    N.write_aiger('test1.aig')


    from pyzz import *
    
    N = netlist() # construct a netlist
    
    wires [ N.add_PI() for _ in xrange(10) ] # create 10 new PIs

    po2 = N.add_PO(fanin=conjunction(N, wires)) # creates a new PO whose fanin is the conjunction of all the PIs
    
    N.write_aiger('test2.aig')

