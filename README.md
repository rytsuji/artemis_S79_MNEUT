# artemis-workdir-template

template of artemis working directory  to be forked by each analysis project


# Example

An example steering file steering/example.tmpl.yaml includes the basic
way to describe processors and histogram definitions. Please look at it.

To run the example steering file, please do at the command prompt of artemis.
No special libraries are required.

artemis> add steering/example.tmpl.yaml NUM=1000 MAX=10
artemis> res

In this run, one thousands of random numbers between 0 and 10 will be
generated in a brach named 'random' and two histogram which contain
the same histogram difinitions will be created.



