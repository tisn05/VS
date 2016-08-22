#! /usr/bin/python
#
# -*- coding: utf-8 -*-
#
#	Copyright (C) 2013 by Igor E. Novikov
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.

import time

from uc2 import uc2_init
from uc2.formats import get_loader

docs = []

def release():
	for doc in docs:
		doc.close()
		docs.remove(doc)
	print 'released'

uc_app = uc2_init()
time.sleep(3)

filepath = 'data/sample.pdxf'
filepath2 = 'data/sample2.pdxf'
loader = get_loader(filepath)
i = 0
while i < 20:
	docs.append(loader(uc_app.appdata, filepath2))
	release()
	docs.append(loader(uc_app.appdata, filepath))
	release()
	docs.append(loader(uc_app.appdata, filepath2))
	release()
	docs.append(loader(uc_app.appdata, filepath))
	release()
	i += 1

print 'Done'
