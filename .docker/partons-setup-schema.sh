#!/bin/bash

__mysql_database="${MYSQL_DATABASE}"
__mysql_user="${MYSQL_USER}"
__mysql_password="${MYSQL_PASSWORD}"

function sql_exec() {
  mysql -u "${__mysql_user}" -D "${__mysql_database}" -p${__mysql_password} -e "${1}"
}

cd /root/sql_schema

#  TODO: if scripts are going to be enumerated this piece of the code
#+       can be automated, therefore will need less further involvement
#+       of anyone.
#+
#+       Desirable naming might look like following:
#+       mysql5_common.sql                      -> mysql5_001_common.sql
#+       mysql5_gpd_layer.sql                   -> mysql5_002_gpd_layer.sql
#+       mysql5_convol_coeff_function_layer.sql -> mysql5_003_convol_coeff_function_layer.sql
#+       mysql5_observable_layer.sql            -> mysql5_004_observable_layer.sql
sql_exec "SOURCE mysql5_common.sql"
sql_exec "SOURCE mysql5_gpd_layer.sql"
sql_exec "SOURCE mysql5_convol_coeff_function_layer.sql"
sql_exec "SOURCE mysql5_observable_layer.sql"
