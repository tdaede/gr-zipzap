/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gnuradio/io_signature.h>
#include "zipzap_sink_impl.h"

#include <linux/input.h>
#include <linux/uinput.h>

namespace gr {
  namespace zipzap {

    zipzap_sink::sptr
    zipzap_sink::make()
    {
      return gnuradio::get_initial_sptr
        (new zipzap_sink_impl());
    }

    /*
     * The private constructor
     */
    zipzap_sink_impl::zipzap_sink_impl()
	 : gr::sync_block("zipzap_sink",
			       gr::io_signature::make(1, 1, sizeof(float)),
			       gr::io_signature::make(0, 0, 0)
			       )
    {
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0) {
            printf("Can't open /dev/input/uinput! Try running as root?\n");
            exit(EXIT_FAILURE);
        }
        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        ioctl(fd, UI_SET_KEYBIT, KEY_UP);
	ioctl(fd, UI_SET_KEYBIT, KEY_DOWN);
	ioctl(fd, UI_SET_KEYBIT, KEY_LEFT);
	ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT);
        struct uinput_user_dev uidev;

        memset(&uidev, 0, sizeof(uidev));

        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "zipzap");
        uidev.id.bustype = BUS_VIRTUAL;
        write(fd, &uidev, sizeof(uidev));
        ioctl(fd, UI_DEV_CREATE);
    }

    /*
     * Our virtual destructor.
     */
    zipzap_sink_impl::~zipzap_sink_impl()
    {
    }

    void zipzap_sink_impl::handle_key(int p)
    {
	 struct input_event ev;
	 memset(&ev, 0, sizeof(ev));
	 ev.type = EV_KEY;

	 ev.code = KEY_UP;
	 ev.value = (p == 10) || (p == 28) || (p == 34);
	 write(fd, &ev, sizeof(ev));

	 ev.code = KEY_DOWN;
	 ev.value = (p == 40) || (p == 52) || (p == 46);
	 write(fd, &ev, sizeof(ev));

	 ev.code = KEY_LEFT;
	 ev.value = (p == 58) || (p == 28) || (p == 52);
	 write(fd, &ev, sizeof(ev));

	 ev.code = KEY_RIGHT;
	 ev.value = (p == 64) || (p == 34) || (p == 46);
	 write(fd, &ev, sizeof(ev));

	 ev.type = EV_SYN;
	 ev.code = 0;
	 ev.value = 0;
	 write(fd, &ev, sizeof(ev));
    }

    int
    zipzap_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];

        // Do <+signal processing+>
	for (int i = 0; i < noutput_items; i++) {
	     if (in[i] > 0.5f) {
		  high++;
	     } else {
		  if (high != 0) {
		       if (high > 200) {
			    // long pulse
			    if (pulse_count != 0) {
				 printf("Pulse count: %d\n",pulse_count);
				 handle_key(pulse_count);
			    }
			    pulse_count = 0;
		       } else {
			    pulse_count++;
		       }
		  }
		  high = 0;
	     }
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace zipzap */
} /* namespace gr */

