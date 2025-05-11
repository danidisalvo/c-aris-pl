# aris-pl

A simple propositional logic (PL) interpreter inspired by Peter Smith's "An Introduction to Formal Logic" (2nd edition - https://www.logicmatters.net/ifl/).


aris-pl defines a set of unary and binary operators and can validate and valuate PL arguments. It can also assert tautologies.

### Unary Operators

**Negation**: `!`, or `~`

### Binary Operators

- **Assignment**: `:=`

- **Conjunction**: `&`

- **Disjunction**: `|`

- **Conditional, or Material implication**: `->`

- **Therefore**: `=>`, or `therefore`

Binary operators have the same precedence, and _must_ always be enclosed in a pair of round brackets.

### Well-Formed Formulae

An **atomic** well-formed formula (wff) _must_ begin with an upper case latin letter (A-Z), 
and it can contain upper case latin letters, digits and the prime `'` character:

- If `P` is a wff, then `!P` is a wff;
- If `P` and `Q` are wff, then `(P & Q)`, `(P | Q)`, and `(P -> Q)` are wff

### Identifiers

An identifier _must_ begin with a lower case latin letter (a-z), and it can contain lower case latin letters and digits 

### Arguments

An argument consists of one or more comma-separated premises, followed by the `therefore` (`=>`) sign, and by a conclusion:

- `(P & Q) => R`

An argument is defined by the `argument` keyword and _must_ be assigned to a variable by the assignment operator `:=`:

`argument a := (P & Q) => R`

##### Validation of an Argument

An argument can be validated as follows:

```
argument a := (P & !Q), (R & !S) => (Q | S)
validate a

argument "(P & !Q), (R & !S) => (Q | S)" is invalid


argument b := (P & !Q) => !(Q & R)
validate b

argument "(P & !Q) => !(Q & R)" is valid
```

##### Valuation of an Argument

An argument can be valuated as follows:

```
P := true
Q := true
R := true
argument a := (P & Q) => R
valuate a

argument "(P & Q) => R" is true
```

### Tautologies

A tautology is an argument consisting solely of a conclusion. A tautology is defined by the `argument` keyword 
and _must_ be assigned to a variable by the assignment operator `:=`:

`argument t := (!(!(P & Q) & !(P & R)) | !(P & (Q | R)))`

##### Assertion of a Tautology

A tautology can be asserted as follows:

```
argument t := (!(!(P & Q) & !(P & R)) | !(P & (Q | R)))
assert t

argument "(!(!(P & Q) & !(P & R)) | !(P & (Q | R)))" is a tautology
```

### Reserved Words 

- `argument`
- `assert`
- `false`
- `print`
- `therefore`
- `true`
- `validate`
- `valuate`

