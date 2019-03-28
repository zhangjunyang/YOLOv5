#!/bin/bash -x

# usage: "$ ./run.sh peripheralservice/mostshadow"

RUN_PATH=`pwd`
OUTPUT_PATH=${RUN_PATH}/output
UNIT_NAME=$1

COMPONENT_NAME=`grep -rn COMPONENT_NAME.*= ${UNIT_NAME}/../*.mk`
ssh tagl@tagl_sysroot "sudo rm -rf /usr/tagl/conf/${COMPONENT_NAME##* }"
ssh tagl@tagl_sysroot "rm -rf ${RUN_PATH/tagldev/tagl}/${UNIT_NAME}"

cd ${RUN_PATH}/${UNIT_NAME}

make clean-test
make test -j8
make install-test-local
make run-ut-local

cd ${RUN_PATH}

ssh tagl@tagl_sysroot "chmod 777 ${RUN_PATH/tagldev/tagl}/${UNIT_NAME} -R"
scp -r tagl@tagl_sysroot:${RUN_PATH/tagldev/tagl}/${UNIT_NAME}/* ${RUN_PATH}/${UNIT_NAME}

mkdir -p ${OUTPUT_PATH}/${UNIT_NAME}

source /opt/poky/2.1.2/environment-setup-core2-64-poky-linux &>/dev/null
lcov -c --gcov-tool x86_64-poky-linux-gcov -d ${RUN_PATH}/${UNIT_NAME} -o ${OUTPUT_PATH}/${UNIT_NAME}/lcov.info
lcov -r ${OUTPUT_PATH}/${UNIT_NAME}/lcov.info "*/usr/*" "*/objs*/*" "*/mock/*" "*/Mock/*" "*/objs/usr/*" "*/test/*" "*/*test_common/*" -o ${OUTPUT_PATH}/${UNIT_NAME}/coverage.info
genhtml --branch-coverage -o ${OUTPUT_PATH}/${UNIT_NAME}/coverage_html_report ${OUTPUT_PATH}/${UNIT_NAME}/coverage.info
