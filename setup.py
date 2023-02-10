from distutils.core import setup, Extension

fields_module = Extension('_fields',
    sources=['fields.c', 'fields_wrap.c'],
)

setup (name = 'fields',
    version = '0.1',
    author      = "Kenny Nguyen",
    description = """calculate the maxwell's equations""",
    ext_modules = [fields_module],
    py_modules = ["fields"],
) 