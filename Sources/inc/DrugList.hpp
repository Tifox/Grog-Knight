/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 * under the License.
 */

/**
 * File: DrugList.hpp
 * Creation: 2015-08-11 17:47
 * Manon Budin <mbudin@student.42.fr>
 */


#ifndef __DrugList__
# define __DrugList__


# include "Log.hpp"
# include "Elements.hpp"
# include <sstream>

# ifdef __APPLE__
#  include "../../Tools/jsoncpp/include/json/json.h"
# else
#  include "json/json.h"
# endif

# include <list>
# include "../../Angel/Angel.h"
# ifndef __Elements__
# include "Elements.hpp"
#endif

class Drug;

class DrugList: public Elements {
public:
	DrugList(void);
	~DrugList(void);

	int		checkExists(std::string);
	Drug*	getDrug(std::string);
	Drug*	getDrugRandom(void);

private:
	std::list<Drug*> _allDrugs;

};

# include "Drug.hpp"

#endif
