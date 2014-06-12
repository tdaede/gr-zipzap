gr-zipzap
=========

GNU Radio block for decoding RC car signals and creating Linux input events

See it in action: https://www.youtube.com/watch?v=1dnWOY4S-zA

Usage
=====

Find your RC transmitter's carrier frequency using gqrx or a GNU radio waterfall. Then open apps/zipzap.grc and set that as the carrier frequency. If not using a RTL-SDR, you will need to change the source block. You may also need to change the thresholds used to detect the on-off keying. Automatic gain control could also be added.

Permissions
===========

Make sure you have permissions to /dev/uinput. The dirty way is:

    sudo chmod 777 /dev/uinput

Latency
=======

Make sure to tune your SDR dongle for low latency. The provided GRC file does this with device parameters.