/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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
//#include "gr_uhd_common.h"
//#include <gr_uhd_usrp_source.h>
#include <gr_io_signature.h>
#include "filter_impl.h"

static const pmt::pmt_t TIME_KEY = pmt::pmt_string_to_symbol("rx_time");


namespace gr {
  namespace filtertag {

    filter::sptr
    filter::make(static uhd::usrp::multi_usrp::sptr _dev)
    {
      return gnuradio::get_initial_sptr (new filter_impl(_dev));
    }

    /*
     * The private constructor
     */
    filter_impl::filter_impl(static uhd::usrp::multi_usrp::sptr _dev)
      : gr_block("filter",
		      gr_make_io_signature(1, 1, sizeof (int)),
		      gr_make_io_signature(1, 1, sizeof (float)))
    {
		
		usrp=_dev;
	}

    /*
     * Our virtual destructor.
     */
    filter_impl::~filter_impl()
    {
    }
	
	int
	filter_impl::getcount()
	{
		return cnt;
	}
	
    void
    filter_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
			ninput_items_required[0] = noutput_items;
    }

    int
    filter_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];
		
		//Get tags in the chunk of data passed in this work function
		uint64_t abs_N, end_N;
		abs_N = nitems_read(0);
		end_N = abs_N + (uint64_t)(noutput_items);
		d_tags.clear();
		get_tags_in_range(d_tags, 0, abs_N, end_N,TIME_KEY);


		//If we get new tags reinitialize cnt and time offset
		if (d_tags.size()>0)
		{
			cnt = (uint64_t)(d_tags.begin()->offset);
			/*
			 * If this is the first time through, initialize time
			 * This if statement should probably get removed eventually to 
			 *to deal with overflows, but I'll worry about that later...
			*/ 
			if (cnt == 0)
			{
				topmt  = (d_tags.begin()->value); //this is a pmt_t
				to = pmt::pmt_to_double(pmt::pmt_nth(0, to))+pmt::pmt_to_double(pmt::pmt_nth(1, to));
			}
		}

		
        
        for (int i=0; i<noutput_items; i++) 
        {
			cnt++;
			if (in[i] == 1)
			{
				//NEEDS TO HAVE A USRP OBJECT FED TO IT.  PASS IT AS PARAMETER???????????????????????????????????????????????????????
				tpps= usrp->get_time_last_pps(0);
				spps= (tpps.to_ticks(1000000));
				delay= (((double)cnt - (double)spps)/1000000);				
			}
			out[i]=delay;
        }
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace filtertag */
} /* namespace gr */

