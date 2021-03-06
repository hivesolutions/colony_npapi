/*
 Hive Colony Framework
 Copyright (c) 2008-2020 Hive Solutions Lda.

 This file is part of Hive Colony Framework.

 Hive Colony Framework is free software: you can redistribute it and/or modify
 it under the terms of the Apache License as published by the Apache
 Foundation, either version 2.0 of the License, or (at your option) any
 later version.

 Hive Colony Framework is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 Apache License for more details.

 You should have received a copy of the Apache License along with
 Hive Colony Framework. If not, see <http://www.apache.org/licenses/>.

 __author__    = João Magalhães <joamag@hive.pt>
 __version__   = 1.0.0
 __revision__  = $LastChangedRevision$
 __date__      = $LastChangedDate$
 __copyright__ = Copyright (c) 2008-2020 Hive Solutions Lda.
 __license__   = Apache License, Version 2.0
*/

#include "stdafx.h"

#ifdef COLONY_PYTHON

#include "python.h"

#define HELLO_WORLD_B64 "SGVsbG8gV29ybGQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAEAAABAAQAAAA\
AAAAAAAABDYWxpYnJpAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACQAAAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
AAAAAAAAAwAAABIZWxsbyBXb3JsZAA="

static PyObject *get_devices(PyObject *self, PyObject *args) {
    /* allocates memory for the various internal structure
    that are going to be used to retrieve device information */
    size_t index;
    size_t devices_s;
    PyObject *element;
    PyObject *item;
    struct device_t *device;
    struct device_t *devices;
    PyObject *result = PyList_New(0);

    /* retrieves the complete set of available printing
    devices and then iterates over them to convert their
    internal structure into dictionaries to be returned */
    pdevices(&devices, &devices_s);
    for(index = 0; index < devices_s; index++) {
        device = &devices[index];
        element = PyDict_New();
        item = PyString_Decode(
            device->name,
            device->name_s,
            "utf-8",
            NULL
        );
        PyDict_SetItemString(element, "name", item);
        item = PyBool_FromLong((long) device->is_default);
        PyDict_SetItemString(element, "is_default", item);
        item = PyString_Decode(
            device->media,
            device->media_s,
            "utf-8",
            NULL
        );
        PyDict_SetItemString(element, "media", item);
        item = PyFloat_FromDouble((double) device->width);
        PyDict_SetItemString(element, "width", item);
        item = PyFloat_FromDouble((double) device->length);
        PyDict_SetItemString(element, "length", item);
        PyList_Append(result, element);
    }

    /* releases the memory that was allocated for the
    device structures sequence (avoids memory leak) */
    free(devices);

    /* returns the list that has been constructed for the
    values that are going to be returned to the caller */
    return result;
}

static PyObject *print_devices(PyObject *self, PyObject *args) {
    /* allocates memory for the various internal structure
    that are going to be used to print device information */
    size_t index;
    size_t devices_s;
    struct device_t *device;
    struct device_t *devices;

    /* retrieves the complete set of available printing
    devices and then iterates over them to print their
    currently set values and settings */
    pdevices(&devices, &devices_s);
    for(index = 0; index < devices_s; index++) {
        device = &devices[index];
        printf("%s\n", device->name);
    }

    /* releases the memory that was allocated for the
    device structures sequence (avoids memory leak) */
    free(devices);

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyObject *print_hello(PyObject *self, PyObject *args) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    size_t data_length;

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
        (unsigned char *) HELLO_WORLD_B64,
        strlen(HELLO_WORLD_B64),
        (unsigned char **) &data,
        &data_length
    );
    print(FALSE, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyObject *print_base64(PyObject *self, PyObject *args) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    char *input;
    size_t data_length;

    /* tries to parse the privided sequence of arguments
    as a single string value that is going to be used as
    the input value for the printing of the page */
    if(PyArg_ParseTuple(args, "s", &input) == FALSE) {
        return NULL;
    }

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
        (unsigned char *) input,
        strlen(input),
        (unsigned char **) &data,
        &data_length
    );
    print(FALSE, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyObject *print_printer_base64(PyObject *self, PyObject *args) {
    /* allocates space for the decoded data buffer and for
    the storage of the length (size) of it */
    char *data;
    char *printer;
    char *input;
    size_t data_length;

    /* tries to parse the privided sequence of arguments
    as a single string value that is going to be used as
    the input value for the printing of the page */
    if(PyArg_ParseTuple(args, "ss", &printer, &input) == FALSE) {
        return NULL;
    }

    /* decodes the data value from the base 64 encoding
    and then uses it to print the data */
    decode_base64(
        (unsigned char *) input,
        strlen(input),
        (unsigned char **) &data,
        &data_length
    );
    print_printer(FALSE, printer, data, data_length);

    /* releases the decoded buffer (avoids memory leak)
    and then returns in success */
    _free_base64((unsigned char *) data);

    /* returns an invalid value to the caller method/function
    as this function should not return anything */
    Py_RETURN_NONE;
}

static PyMethodDef colony_methods[] = {
    {"get_devices", get_devices, METH_NOARGS, "Retrieves the complete set of devices."},
    {"print_devices", print_devices, METH_NOARGS, "Prints the complete set of devices."},
    {"print_hello", print_hello, METH_NOARGS, "Prints an hello message to printer."},
    {"print_base64", print_base64, METH_VARARGS, "Prints a base 64 based sequence of data."},
    {"print_printer_base64", print_printer_base64, METH_VARARGS, "Prints a base 64 based sequence of data in a specific printer."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initnpcolony() {
    (void) Py_InitModule("npcolony", colony_methods);
}

#endif
