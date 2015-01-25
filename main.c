#include <stdio.h>
#include <libusb.h>
#include <signal.h>
#include <string.h>

#define URB_BULK_OUT 0x01
#define URB_BULK_IN  0x82

static int done = 0;
static int card = -1;

void signal_callback_handler(int signum) {
    (void) signum;
    done = 1;
}

void dump_buffer(unsigned char* buffer, int length) {
    for (int i = 0; i < length; i++) {
        if (i > 0) {
            fprintf(stderr, ":");
        }
        fprintf(stderr, "%02x", buffer[i]);
    }
    fprintf(stderr, "\n");
}

void handle_10(struct libusb_device_handle *devh, unsigned char* buffer_05, int _actual_length) {
    int actual_length = _actual_length;
    if (buffer_05[7] == 0x02) {
        unsigned char expected_result_05[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x02, 0x00, 0x01};
        if (memcmp(buffer_05, expected_result_05, 10) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 10);
        }
        if (card != 0) {
            fprintf(stderr, "No card\n");
        }
        card = 0;
    } else if (buffer_05[7] == 0x01) {
        unsigned char expected_result_05[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x01, 0x00, 0x01};
        if (memcmp(buffer_05, expected_result_05, 10) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 10);
        }
        if (card != 1) {
            fprintf(stderr, "Card!\n");

            /* -------------------------------------------------------------------- */
            /* Card query */
            /* -------------------------------------------------------------------- */

            unsigned char data_06[10] = {0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
            unsigned char buffer_06[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_06, 10, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_06, 271, &actual_length, 1000);
            if (actual_length != 23) {
                fprintf(stderr, "06: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_06[10] = {0x80, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
                if (memcmp(buffer_06, expected_result_06, 10) != 0) {
                    fprintf(stderr, "buffer_06: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_06, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_06, 10);
                }
            }
            for (int i = 0; i < actual_length; i++) {
                fprintf(stdout, "%xd:", buffer_06[10 + i]);
            }
            fprintf(stdout, "\n");

            unsigned char data_07[11] = {0x6b, 0x01, 0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0xda};
            unsigned char buffer_07[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_07, 11, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_07, 271, &actual_length, 1000);
            if (actual_length != 31) {
                fprintf(stderr, "07: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_07[31] = {0x83, 0x15, 0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x68, 0xf0, 0xeb, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x08, 0xfe, 0x00, 0x28, 0x04};
                if (memcmp(buffer_07, expected_result_07, 31) != 0) {
                    fprintf(stderr, "buffer_07: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_07, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_07, 31);
                }
            }

            unsigned char data_08[10] = {0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
            unsigned char buffer_08[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_08, 10, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_08, 271, &actual_length, 1000);
            if (actual_length != 10) {
                fprintf(stderr, "08: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_08[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x01};
                if (memcmp(buffer_08, expected_result_08, 10) != 0) {
                    fprintf(stderr, "buffer_08: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_08, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_08, 10);
                }
            }
        }
        card = 1;
    } else if (buffer_05[7] == 0x00) {
        unsigned char expected_result_05[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00};
        if (memcmp(buffer_05, expected_result_05, 10) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 10);
        }

        unsigned char data_06[12] = {0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
        unsigned char buffer_06[271] = {0};
        libusb_bulk_transfer(devh, URB_BULK_OUT, data_06, 12, &actual_length, 1000);
        libusb_bulk_transfer(devh, URB_BULK_IN, buffer_06, 271, &actual_length, 1000);
        if (actual_length != 10) {
            fprintf(stderr, "06: size differs %d\n", actual_length);
        } else {
            unsigned char expected_result_06[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00};
            if (memcmp(buffer_06, expected_result_06, 10) != 0) {
                fprintf(stderr, "buffer_06: Missmatch detected!\nGot:      ");
                dump_buffer(buffer_06, actual_length);
                fprintf(stderr, "Expected: ");
                dump_buffer(expected_result_06, 10);
            }
        }
    } else {
        fprintf(stderr, "Unknown response 10\n");
        dump_buffer(buffer_05, actual_length);
    }
}

void handle_11(struct libusb_device_handle *devh, unsigned char* buffer_05, int _actual_length) {
    int actual_length = _actual_length;
    if (buffer_05[7] == 0x02) {
        unsigned char expected_result_05[11] = {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x02, 0x00, 0x00, 0x01};
        if (memcmp(buffer_05, expected_result_05, 11) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 11);
        }
        if (card != 0) {
            fprintf(stderr, "No card\n");
        }
        card = 0;
    } else if (buffer_05[7] == 0x01) {
        unsigned char expected_result_05[11] = {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x01, 0x00, 0x00, 0x01};
        if (memcmp(buffer_05, expected_result_05, 11) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 11);
        }
        if (card != 1) {
            fprintf(stderr, "Card!\n");

            /* -------------------------------------------------------------------- */
            /* Card query */
            /* -------------------------------------------------------------------- */

            unsigned char data_06[10] = {0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
            unsigned char buffer_06[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_06, 10, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_06, 271, &actual_length, 1000);
            if (actual_length != 10) {
                fprintf(stderr, "06: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_06[10] = {0x80, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
                if (memcmp(buffer_06, expected_result_06, 10) != 0) {
                    fprintf(stderr, "buffer_06: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_06, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_06, 10);
                }
            }
            for (int i = 0; i < actual_length; i++) {
                fprintf(stdout, "%xd:", buffer_06[10 + i]);
            }
            fprintf(stdout, "\n");

            unsigned char data_07[11] = {0x6b, 0x01, 0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0xda};
            unsigned char buffer_07[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_07, 11, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_07, 271, &actual_length, 1000);
            if (actual_length != 31) {
                fprintf(stderr, "07: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_07[31] = {0x83, 0x15, 0x00, 0x00, 0x00, 0x00, 0xda, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x68, 0xf0, 0xeb, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0x08, 0xfe, 0x00, 0x28, 0x04};
                if (memcmp(buffer_07, expected_result_07, 31) != 0) {
                    fprintf(stderr, "buffer_07: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_07, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_07, 31);
                }
            }

            unsigned char data_08[10] = {0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
            unsigned char buffer_08[271] = {0};
            libusb_bulk_transfer(devh, URB_BULK_OUT, data_08, 10, &actual_length, 1000);
            libusb_bulk_transfer(devh, URB_BULK_IN, buffer_08, 271, &actual_length, 1000);
            if (actual_length != 10) {
                fprintf(stderr, "08: size differs %d\n", actual_length);
            } else {
                unsigned char expected_result_08[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x01};
                if (memcmp(buffer_08, expected_result_08, 10) != 0) {
                    fprintf(stderr, "buffer_08: Missmatch detected!\nGot:      ");
                    dump_buffer(buffer_08, actual_length);
                    fprintf(stderr, "Expected: ");
                    dump_buffer(expected_result_08, 10);
                }
            }
        }
        card = 1;
    } else if (buffer_05[7] == 0x00) {
        unsigned char expected_result_05[11] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00};
        if (memcmp(buffer_05, expected_result_05, 11) != 0) {
            fprintf(stderr, "buffer_05: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_05, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_05, 11);
        }

        unsigned char data_06[12] = {0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
        unsigned char buffer_06[271] = {0};
        libusb_bulk_transfer(devh, URB_BULK_OUT, data_06, 12, &actual_length, 1000);
        libusb_bulk_transfer(devh, URB_BULK_IN, buffer_06, 271, &actual_length, 1000);
        if (actual_length != 10) {
            fprintf(stderr, "06: size differs %d\n", actual_length);
        } else {
            unsigned char expected_result_06[10] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00};
            if (memcmp(buffer_06, expected_result_06, 10) != 0) {
                fprintf(stderr, "buffer_06: Missmatch detected!\nGot:      ");
                dump_buffer(buffer_06, actual_length);
                fprintf(stderr, "Expected: ");
                dump_buffer(expected_result_06, 10);
            }
        }
    } else {
        fprintf(stderr, "Unknown response 11\n");
        dump_buffer(buffer_05, actual_length);
    }
}

int main() {
    signal(SIGINT, signal_callback_handler);

    static struct libusb_device_handle *devh = NULL;
    uint16_t vendor_id;
    uint16_t product_id;

    vendor_id = 0x04e6;
    product_id = 0x5292;

    int retval = libusb_init(NULL);
    if (retval < 0) {
        fprintf(stderr, "Error initializing libusb %d\n", retval);
        return 1;
    }
    devh = libusb_open_device_with_vid_pid(NULL, vendor_id, product_id);
    if (devh == NULL) {
        /* ACTION=="add|change", SUBSYSTEM=="usb", ATTRS{idVendor}=="04e6", ATTRS{idProduct}=="5292", GROUP="wheel" */
        fprintf(stderr, "Error opening device, is the device connected and necessary permissions?\n");
        return 1;
    }
    libusb_detach_kernel_driver(devh, 0);
    libusb_claim_interface(devh, 0);

    int actual_length;

    /* -------------------------------------------------------------------- */
    /* Device init */
    /* -------------------------------------------------------------------- */

    unsigned char data_01[12] = {0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x02, 0xfe, 0xd4, 0x02, 0x2a, 0x00};
    unsigned char buffer_01[2160] = {0};
    libusb_bulk_transfer(devh, URB_BULK_OUT, data_01, 12, &actual_length, 1000);
    libusb_bulk_transfer(devh, URB_BULK_IN, buffer_01, 2160, &actual_length, 1000);
    if (actual_length != 10) {
        fprintf(stderr, "01: size differs %d\n", actual_length);
    } else {
        // Sometimes this is the answer: {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x02, 0x00, 0x01}, driver continues anyway
        unsigned char expected_result_01[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7e, 0x00, 0x02};
        if (memcmp(buffer_01, expected_result_01, 10) != 0) {
            fprintf(stderr, "buffer_01: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_01, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_01, 10);
        }
    }

    unsigned char data_02[11] = {0x6b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x13};
    unsigned char buffer_02[271] = {0};
    libusb_bulk_transfer(devh, URB_BULK_OUT, data_02, 11, &actual_length, 1000);
    libusb_bulk_transfer(devh, URB_BULK_IN, buffer_02, 271, &actual_length, 1000);
    if (actual_length != 11) {
        fprintf(stderr, "02: size differs %d\n", actual_length);
    } else {
        // Sometimes this is the answer: {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x02, 0x00, 0x01}, driver continues anyway
        unsigned char expected_result_02[11] = {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x13, 0x02, 0x00, 0x00, 0x81};
        if (memcmp(buffer_02, expected_result_02, 11) != 0) {
            fprintf(stderr, "buffer_02: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_02, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_02, 11);
        }
    }

    unsigned char data_03[11] = {0x6b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e};
    unsigned char buffer_03[271] = {0};
    libusb_bulk_transfer(devh, URB_BULK_OUT, data_03, 11, &actual_length, 1000);
    libusb_bulk_transfer(devh, URB_BULK_IN, buffer_03, 271, &actual_length, 1000);
    if (actual_length != 48) {
        fprintf(stderr, "03: size differs %d\n", actual_length);
    } else {
        // Sometimes this is the answer: {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x13, 0x02, 0x00, 0x00, 0x81}, driver continues anyway
        unsigned char expected_result_03[48] = {0x83, 0x26, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x02, 0x00, 0x00, 0x01, 0x09, 0x00, 0x03, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x1c, 0x32, 0x00, 0x31, 0x00, 0x31, 0x00, 0x36, 0x00, 0x31, 0x00, 0x30, 0x00,
                                                0x33, 0x00, 0x39, 0x00, 0x32, 0x00, 0x34, 0x00, 0x38, 0x00, 0x37, 0x00, 0x35, 0x00, 0x35, 0x00};
        if (memcmp(buffer_03, expected_result_03, 48) != 0) {
            fprintf(stderr, "buffer_03: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_03, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_03, 48);
        }
    }

    unsigned char data_04[11] = {0x6b, 0x01, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x00, 0x00, 0x00, 0xdb};
    unsigned char buffer_04[271] = {0};
    libusb_bulk_transfer(devh, URB_BULK_OUT, data_04, 11, &actual_length, 1000);
    libusb_bulk_transfer(devh, URB_BULK_IN, buffer_04, 271, &actual_length, 1000);
    if (actual_length != 11) {
        fprintf(stderr, "04: size differs %d\n", actual_length);
    } else {
        // Sometimes this is the answer: {0x83, 0x26, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x02, 0x00, 0x00, 0x01, 0x09, 0x00, 0x03, 0x00, 0x00,
        //                                0x00, 0x00, 0x00, 0x1c, 0x32, 0x00, 0x31, 0x00, 0x31, 0x00, 0x36, 0x00, 0x31, 0x00, 0x30, 0x00,
        //                                0x33, 0x00, 0x39, 0x00, 0x32, 0x00, 0x34, 0x00, 0x38, 0x00, 0x37, 0x00, 0x35, 0x00, 0x35, 0x00}, driver continues anyway
        unsigned char expected_result_04[11] = {0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x02, 0x00, 0x00, 0x01};
        if (memcmp(buffer_04, expected_result_04, 11) != 0) {
            fprintf(stderr, "buffer_04: Missmatch detected!\nGot:      ");
            dump_buffer(buffer_04, actual_length);
            fprintf(stderr, "Expected: ");
            dump_buffer(expected_result_04, 11);
        }
    }

    /* -------------------------------------------------------------------- */
    /* Device query */
    /* -------------------------------------------------------------------- */

    unsigned char data_05[10] = {0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00};
    unsigned char buffer_05[271] = {0};
    while (!done) {
        libusb_bulk_transfer(devh, URB_BULK_OUT, data_05, 10, &actual_length, 1000);
        libusb_bulk_transfer(devh, URB_BULK_IN, buffer_05, 271, &actual_length, 1000);
        if (actual_length == 11) {
            handle_11(devh, buffer_05, actual_length);
        } else if (actual_length == 10) {
            handle_10(devh, buffer_05, actual_length);
        } else {
            // TODO this seems to be a device reset answer:
            // 0x80, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3b, 0x88, 0x80, 0x01, 0x00, 0x73, 0xc8, 0x40, 0x13, 0x00, 0x90, 0x00, 0x71

            fprintf(stderr, "05: size differs %d\n", actual_length);
        }
    }

    fprintf(stderr, "Hallo\n");
    libusb_close(devh);
    libusb_exit(NULL);

    return 0;
}

