"""
Copyright (c) Riskaware 2015

This file is part of OpenEAGGR.

OpenEAGGR is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenEAGGR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

A copy of the GNU Lesser General Public License is available in COPYING.LESSER
or can be found at <http://www.gnu.org/licenses/>.
"""

from setuptools import setup
from codecs import open
from os import path
from _ast import __version__

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'DESCRIPTION.rst'), encoding='utf-8') as f:
    long_description = f.read()
with open(path.join(here, 'eaggr', 'version.py'), encoding='utf-8') as f:
    exec(f.read())

setup(
    name='eaggr',
    version=__version__,

    description='EAGGR Python Library',
    long_description=long_description,

    url='http://www.riskaware.co.uk',

    author='Riskaware Ltd',
    author_email='ian.bush@riskaware.co.uk',

    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Scientific/Engineering',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.4',
    ],
    keywords=['eaggr','dggs',],

    packages=[
        'eaggr',
        'eaggr.enums',
        'eaggr.exceptions',
        'eaggr.shapes',
        'eaggr.dlls.win32-x86',
        'eaggr.dlls.win32-x86-64',
        'eaggr.dlls.linux-x86-64',
    ],
    install_requires=[],
    extras_require={},

    package_data={
        'eaggr.dlls.win32-x86' : ['*.dll'],
        'eaggr.dlls.win32-x86-64' : ['*.dll'],
        'eaggr.dlls.linux-x86-64' : ['*.so*'],
    },
    data_files=[],

    entry_points={},
)
