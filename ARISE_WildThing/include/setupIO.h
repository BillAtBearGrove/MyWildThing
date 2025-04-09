#pragma once

/*
 * Setup the GPIO and ADCs.
 * Does not touch the pins related to the VNH7070AY,
 * its driver will configure those pins.
 */
void setup_io();