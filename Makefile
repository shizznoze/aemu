ATPRO = atpro
MEMAB = memab
PSPNET_ADHOC_AUTH = pspnet_adhoc_auth
PSPNET_ADHOC = pspnet_adhoc
PSPNET_ADHOCCTL = pspnet_adhocctl
PSPNET_ADHOC_MATCHING = pspnet_adhoc_matching
PSPNET_ADHOC_DOWNLOAD = pspnet_adhoc_download
PSPNET_ADHOC_DISCOVER = pspnet_adhoc_discover
PSPNET_ADHOCCTL_SERVER = pspnet_adhocctl_server
DISTRIBUTE = dist

FLAGS = CONFIG_620=1

ifeq ($(CONFIG_63X), 1)
FLAGS = CONFIG_63X=1
endif

ifeq ($(CONFIG_660), 1)
FLAGS = CONFIG_660=1
endif

ifeq ($(RELEASE), 1)
FLAGS += RELEASE=1
else
FLAGS += DEBUG=1
endif

ifeq ($(ENABLE_PEERLOCK), 1)
FLAGS += ENABLE_PEERLOCK=1
endif

ifeq ($(ENABLE_NETLOCK), 1)
FLAGS += ENABLE_NETLOCK=1
endif

ifeq ($(PDP_DIRTY_MAGIC), 1)
FLAGS += PDP_DIRTY_MAGIC=1
endif

all:
	@mkdir $(DISTRIBUTE) || true
	@mkdir $(DISTRIBUTE)/seplugins || true
	@mkdir $(DISTRIBUTE)/kd || true
	@mkdir $(DISTRIBUTE)/server || true
	@touch $(DISTRIBUTE)/seplugins/hotspot.txt || true
	echo 81.217.28.171 > $(DISTRIBUTE)/seplugins/server.txt
#	@touch $(DISTRIBUTE)/seplugins/server.txt || true

	@cd $(ATPRO); make $(FLAGS)
	@cd $(MEMAB); make $(FLAGS)
	@cd $(PSPNET_ADHOC_AUTH); make $(FLAGS)
	@cd $(PSPNET_ADHOC); make $(FLAGS)
	@cd $(PSPNET_ADHOCCTL); make $(FLAGS)
	@cd $(PSPNET_ADHOC_MATCHING); make $(FLAGS)
	@cd $(PSPNET_ADHOC_DOWNLOAD); make $(FLAGS)
	@cd $(PSPNET_ADHOC_DISCOVER); make $(FLAGS)
	@cd $(PSPNET_ADHOCCTL_SERVER); make $(FLAGS)

clean:
	@cd $(ATPRO); make clean $(FLAGS)
	@cd $(MEMAB); make clean $(FLAGS)
	@cd $(PSPNET_ADHOC_AUTH); make clean $(FLAGS)
	@cd $(PSPNET_ADHOC); make clean $(FLAGS)
	@cd $(PSPNET_ADHOCCTL); make clean $(FLAGS)
	@cd $(PSPNET_ADHOC_MATCHING); make clean $(FLAGS)
	@cd $(PSPNET_ADHOC_DOWNLOAD); make clean $(FLAGS)
	@cd $(PSPNET_ADHOC_DISCOVER); make clean $(FLAGS)
	@cd $(PSPNET_ADHOCCTL_SERVER); make clean $(FLAGS)
	@rm -rf $(DISTRIBUTE) || true
