compile: clean
	particle compile core ../firmware
	mv *.bin core_firmware.bin

flash: compile
	particle flash --usb core_firmware.bin --force

flash_cloud: compile
	particle flash core_firmware.bin

monitor:
	particle serial monitor

clean:
	rm -rf *.bin