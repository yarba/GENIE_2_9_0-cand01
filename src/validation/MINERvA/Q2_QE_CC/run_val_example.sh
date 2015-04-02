#!/bin/bash

#if [ "x" == "x$GENIE" ] ; then
#source ${WORKDIR}/environment_setup.sh
#fi

MCINPUT=mc_input.xml
if [ -e ${MCINPUT} ]; then
rm -f ${MCINPUT}
fi

splinefile=`ls -AlF ${XSECSPLINEDIR}/*.xml | awk '{print $NF}'`

# gevgen -r 1 -n 10000 -p 14 -e 1.5,10 -t1000010010[0.077418],1000060120[0.922582] \
#--cross-sections ./splines/gxspl-vA-v2.8.0.xml \
# -f ./flux-MINERvA/nu-flux-MINERvA.data
#
gevgen -r 1 -n 10000 -p 14 -e 1.5,10 -t1000010010[0.077418],1000060120[0.922582] --cross-sections $splinefile -f ./flux-MINERvA/nu-flux-MINERvA.data

printf "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" >> ${MCINPUT}
printf "<genie_simulation_outputs>\n"                      >> ${MCINPUT}
printf "<model name=\"${GENIRVER}\">\n"                    >> ${MCINPUT}
printf "<evt_file format=\"ghep\"> ./gntp.1.ghep.root </evt_file>\n" >> ${MCINPUT}
printf "</model>\n"                                        >> ${MCINPUT}
printf "</genie_simulation_outputs>"                       >> ${MCINPUT}

./gvld_MINERvA_Q2_QE_CC -g mc_input.xml -d MINERvA_expdata_input.xml

