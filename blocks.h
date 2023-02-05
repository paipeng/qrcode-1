/*\
 *  Yes, This Is Another Barcode Reader
 *  Copyright (C) 2013 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef BLOCKS_H
#define BLOCKS_H

#include "scanner.h"
#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
unsigned int get_bits(scanner_t* scanner, size_t n);
void         put_bits(scanner_t* scanner, size_t n, const char* stream);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif
