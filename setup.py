#!/usr/bin/python3
from distutils.core import setup, Extension

fields_module = Extension('_fields',
    sources=[
        'process.c'
        , 'fields_wrap.c'
        , 'vector.c'
        , 'kernel.c'
        , 'world_constants.c'
        , 'accessor.c'
        , 'field.c'
    ],
)

setup (name = 'fields',
    version = '0.1',
    author      = "Kenny Nguyen",
    description = """calculate the maxwell's equations""",
    ext_modules = [fields_module],
    py_modules = ["fields"],
) 