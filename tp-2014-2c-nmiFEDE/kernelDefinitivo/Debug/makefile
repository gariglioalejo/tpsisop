################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: kernelDefinitivo

dependents:
	-cd /home/utnso/tp-2014-2c-nmi/tpsisoplib/Debug && $(MAKE) all

# Tool invocations
kernelDefinitivo: $(OBJS) $(USER_OBJS) /home/utnso/tp-2014-2c-nmi/tpsisoplib/Debug/libtpsisoplib.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2014-2c-nmi/tpsisoplib/Debug" -o "kernelDefinitivo" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) kernelDefinitivo
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2014-2c-nmi/tpsisoplib/Debug/libtpsisoplib.so:

-include ../makefile.targets
