#!/bin/bash
#For tagl_dev:22.2

RUN_VERSION="latest"
SYSROOT_RUN_VERSION="latest"
CONTAINER_NAME="tagl"
SYSROOT_CONTAINER_NAME="tagl_sysroot"
SYSROOT_LINK_NAME="sysroot"
TARGET_IMAGE="registry.cr.toyota.co.jp/tagl_dev_ext_eb:"
SYSROOT_TARGET_IMAGE="registry.cr.toyota.co.jp/tagl_unittest:"

pushd . 1>/dev/null
cd 1>/dev/null
HOME_DIR=`pwd`
popd 1>/dev/null
SSH_DIR=${HOME_DIR}/.ssh
TAGL_HOME=${HOME_DIR}/zjy
RUN_UID=`id -u`
RUN_GID=`id -g`
 
CMDNAME=`basename $0`

print_usage(){
    echo "Usage: $CMDNAME [-v VERSION][-n CONTAINER_NAME][--force-rm]" 1>&2
    echo "Options:" 1>&2
    echo "  -v : Create tagl_dev container with indicated version" 1>&2
    echo "  -n :  Create container with indicated name" 1>&2
    echo "  -f : Remove existing container" 1>&2
}

while getopts v:n:fh OPT
do
    case $OPT in
        "h" ) print_usage
              exit 0;;
        "v" ) FLG_V="TRUE" RUN_VERSION="$OPTARG" SYSROOT_RUN_VERSION="$OPTARG";;
        "n" ) FLG_N="TRUE" CONTAINER_NAME="$OPTARG";;
        "f" ) FLG_F="TRUE" ;;
          * ) print_usage
              exit 1;;
    esac
done

if [ "$FLG_F" = "TRUE" ]; then
    echo "Are you sure to remove existing container?[Y/N]"
    read answer
    case $answer in
    [yY]) 
        docker stop ${CONTAINER_NAME} ${SYSROOT_CONTAINER_NAME} 1>&2>/dev/null
        docker rm ${CONTAINER_NAME} ${SYSROOT_CONTAINER_NAME} 1>&2>/dev/null
        docker volume ls -qf dangling=true | xargs -r docker volume rm 2>&1 > /dev/null
        ;;
    [nN])
        exit;
    esac
fi

docker pull ${TARGET_IMAGE}${RUN_VERSION} 2>/dev/null
docker pull ${SYSROOT_TARGET_IMAGE}${SYSROOT_RUN_VERSION} 2>&1 > /dev/null

mkdir ${TAGL_HOME} 2>/dev/null
if [ ! -e ${SSH_DIR} ]; then
   ssh-keygen -f ${SSH_DIR}/id_rsa -t rsa -N ""
fi
cp -rf ${SSH_DIR} ${TAGL_HOME} 2>/dev/null
cp ${HOME_DIR}/.bashrc ${TAGL_HOME} 2>/dev/null
#chmod 777 -R ${TAGL_HOME} 2>/dev/null
if [ ! -e ${TAGL_HOME}/.ssh/config ]; then
   touch ${TAGL_HOME}/.ssh/config
fi
grep ${SYSROOT_CONTAINER_NAME} ${TAGL_HOME}/.ssh/config 2>&1 > /dev/null || \
        echo -e "Host ${SYSROOT_CONTAINER_NAME}\n  StrictHostKeyChecking no" >> ${TAGL_HOME}/.ssh/config

# for TestDocker
docker run \
-d --privileged \
--ulimit msgqueue=81920000:81920000 \
--name ${SYSROOT_CONTAINER_NAME} \
-u=${RUN_UID}:${RUN_GID} \
-v ${TAGL_HOME}:/home/testuser \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /etc/localtime:/etc/localtime:ro \
-v /dev/snd:/dev/snd:ro \
-v sysroot_container:/usr/tagl \
-e DISPLAY=$DISPLAY \
${SYSROOT_TARGET_IMAGE}${SYSROOT_RUN_VERSION} 2>&1 > /dev/null

# for BuildDocker
docker run \
-it --privileged \
--ulimit msgqueue=81920000:81920000 \
--name ${CONTAINER_NAME} \
-u=${RUN_UID}:${RUN_GID} \
-v ${TAGL_HOME}:/home/tagldev \
-v /tmp/.X11-unix:/tmp/.X11-unix \
-v /etc/localtime:/etc/localtime:ro \
-v /dev/snd:/dev/snd:ro \
--link ${SYSROOT_CONTAINER_NAME} \
--volumes-from ${SYSROOT_CONTAINER_NAME} \
-e DISPLAY=$DISPLAY \
${TARGET_IMAGE}${RUN_VERSION}

