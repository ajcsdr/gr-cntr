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

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/types/device_addr.hpp>
#include <uhd/types/io_type.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <complex>
#include <iomanip> //setw() set field width
#include <gr_io_signature.h>
#include <gr_uhd_usrp_source.h>

#include <gr_io_signature.h>
#include "magic_impl.h"
#include <gr_core_api.h>
#include <iomanip> //setw() set field width
#include <gr_uhd_api.h>
#include <gr_sync_block.h>
#include <uhd/usrp/multi_usrp.hpp>
//#include "time_spec.hpp"

namespace gr {
  namespace MyModules3 {
	//uhd::device_addr_t device_addr=uhd::device_addr_t::device_addr_t(std::string "");
    magic::sptr
    magic::make(float sample_rate)
    {

      return gnuradio::get_initial_sptr (new magic_impl(sample_rate));
    }

    /*
     * The private constructor
     */
	//uhd::usrp::multi_usrp::sptr device;
    magic_impl::magic_impl(float sample_rate)
      : gr_sync_block("magic",
		      gr_make_io_signature(1, 1, sizeof (float)),
		      gr_make_io_signature(1, 1, sizeof (float)))
    {
		std::string args;
		uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);


		//device = uhd::usrp::multi_usrp::make();//uhd::device_addr_t "");
		}

    /*
     * Our virtual destructor.
     */
    magic_impl::~magic_impl()
    {
    }
/**********************************************************************/
    
 std::vector<gr_tag_t> d_tags; //declare d_tags as a vector of gr_tag_t objects variable
 gr_tag_t d_tag;				//declare d_tag as a gr_tag_t
std::string d_name;				//declare d_name as a string
std::vector<gr_tag_t>::iterator d_tags_itr;		//declare d_tags_itr as an iterator
uint64_t abs_N, end_N;			//declare abs_N and end_N as unsigned integer 64
int chunk_cnt = 0;				//declare and define count =0;
int samp_cnt = 0;				//declare and define samp_cnt = 0
int tag_grp_cnt = 0;				//declare and define tag_cnt =0    


uint64_t last_tag_samp = 0, cur_tag_samp = 0, samp_btween_tags = 0, samp_error = 0;
double_t last_tag_time = 0, cur_tag_time = 0, time_btween_tags=0;
double_t mysample_rate;

//noutput_items is the number of items your block is *expected* to produce
// nitems_read and nitems_written. It returns a 64-bit number of the item count since the start of the flow graph


int
magic_impl::work(int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items)
{
    const float *in = (const float *) input_items[0];
    float *out = (float *) output_items[0];

    abs_N = nitems_read(0);
    end_N = abs_N + (uint64_t)(noutput_items);
	
	if (tag_grp_cnt < 10)	//only display the first 10 times a group of tags are seen (NOT a counter of individual tags received)
	{				
		
     
		d_tags.clear();
		get_tags_in_range(d_tags, 0, abs_N, end_N); //hard code up abs_N and end_N
    
		if (d_tags.size() > 0){//the number of tag objects held within the d_tags vector usually 3 index0-2 (rx_time, rx_rate, rx_freq)
			std::cout <<std::endl <<	"Search For Tags Start: " << abs_N 
					  << std::setw(15)<<"  Search For Tags End: " << end_N 
					  << std::setw(15)<<"  Tags FOUND" << std::endl;
			//DISPLAY TAG INFORMATION OF THE FIRST 3 TAGS RECEIVED------------------------------------------------------------------------------------------------------------------------------------------------------
			std::cout << std::fixed;
			std::cout <<"Input Stream: " << 0 << "  Tag Vector Size: " <<d_tags.size() << "              noutput_items  " << noutput_items << std::endl;
			const gr_tag_t d_tag0 = d_tags[0];//get tag associated with key:rx_time
			uint64_t cur_tag_samp = d_tag0.offset;
			std::string mystr01 = pmt::pmt_symbol_to_string(d_tag0.srcid);
			std::string mystr02 = pmt::pmt_symbol_to_string(d_tag0.key);
			//(d_tag0.value)is a pmt data type containing one tuple.  The tuple contains two items, whole seconds and fractional seconds.
			//To display contents of tuple: std::cout<< d_tag0.value or to use a variable pmt::pmt_t mytuple = d_tag0.value;  then call std::cout<< mytuple
			//To get at the contents of the tuple do the following:
			pmt::pmt_t mytuple = d_tag0.value;  
			uint64_t ti = pmt::pmt_to_uint64(pmt::pmt_tuple_ref(mytuple,0));
			double_t tf = pmt::pmt_to_double(pmt::pmt_tuple_ref(mytuple,1));
			double_t cur_tag_time = (ti+tf);
			std::cout << std::setw(12) << "Offset: " << cur_tag_samp << std::setw(12) <<" Srcid: " << mystr01 << std::setw(12) << " Key: " << mystr02 << std::setw(19) << " Tag Time(us): " << cur_tag_time << std::endl;

			const gr_tag_t d_tag1 = d_tags[1];//get tag associated with key:rx_rate
			uint64_t offset1 = d_tag1.offset;
			std::string mystr11 = pmt::pmt_symbol_to_string(d_tag1.srcid);
			std::string mystr12 = pmt::pmt_symbol_to_string(d_tag1.key);
			mysample_rate = pmt::pmt_to_double(d_tag1.value);//not a string
			std::cout << std::setw(12)<< "Offset: " << offset1 <<  std::setw(12) << " Srcid: " << mystr11 << std::setw(12)<<  " Key: " << mystr12 << std::setw(12)<<  "Value: " << mysample_rate << std::endl;

			const gr_tag_t d_tag2 = d_tags[2];//get tag associated with key:rx_freq
			uint64_t offset2 = d_tag2.offset;
			std::string mystr21 = pmt::pmt_symbol_to_string(d_tag2.srcid);
			std::string mystr22 = pmt::pmt_symbol_to_string(d_tag2.key);
			double_t myd2 = pmt::pmt_to_double(d_tag2.value);//not a string
			std::cout << std::setw(12) << "Offset: " << offset2 << std::setw(12) <<" Srcid: " << mystr21 << std::setw(12) <<" Key: " << mystr22 << std::setw(12) << "Value: " << myd2 << std::endl;
			
			//const gr_tag_t d_tag3 = d_tags[3];//what is this - offset is always zero
			//uint64_t offset3 = d_tag3.offset;
			//std::cout << "Offset: " << offset3 << std::endl;
			
			//Using an interator to go through the elements of the tag vector			
			//for(d_tags_itr = d_tags.begin(); d_tags_itr != d_tags.end(); d_tags_itr++) {
				//std::cout 
				//<< std::setw(10)<< "Offset: " << d_tags_itr->offset
				//<< std::setw(10)<< "Source: " << (pmt::pmt_is_symbol(d_tags_itr->srcid) ?  pmt::pmt_symbol_to_string(d_tags_itr->srcid) : "n/a")
				//<< std::setw(10)<< "Key: " << pmt::pmt_symbol_to_string(d_tags_itr->key) 
				//<< std::setw(10)<< "Value: " << d_tags_itr->value << std::endl;
			//}//end if End for loop  through d_tags			
			//DONE DISPLAYING TAG INFORMATION-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			
			if (cur_tag_samp == 0){				//first tag received signifing start of flow graph.  Initalize last_tag_samp and last_tag_time
				last_tag_samp = cur_tag_samp;
				last_tag_time = cur_tag_time;
			}
			else{									//calulate the correct ref sample value from the tag time
				time_btween_tags = cur_tag_time - last_tag_time;
				samp_btween_tags = time_btween_tags * mysample_rate;
				samp_error = samp_btween_tags - (cur_tag_samp - last_tag_samp);
			}	
			
			std::cout<<"Last tags sample #:     " 	<< std::setw(15)<< last_tag_samp 				<<"  Last tags time:       "<< std::setw(15)<< last_tag_time 	<< std::endl;
			std::cout<<"Current tags sample #:  " 	<< std::setw(15)<< cur_tag_samp 				<<"  Current tags time:    "<< std::setw(15)<< cur_tag_time 	<< std::endl;
			std::cout<<"Rcved Samp Between Tags:" 	<< std::setw(15)<< cur_tag_samp-last_tag_samp	<<"  Time Between Tags:    "<< std::setw(15)<< time_btween_tags	<< std::endl<< std::endl;
			
			std::cout<<"Theoretical Samples Between Tags: "<< samp_btween_tags 			<<"  Lost Samples: "	<< samp_error				<< std::endl;
		    std::cout<<"Current Count:  "<< std::setw(15)<< samp_cnt	<<"  Count Should Be: "	<< samp_cnt + samp_error	<< std::endl;
						
			samp_cnt = (samp_cnt + samp_error);				//update sample count to reflect what the acutal sample count is taking into account lost samples
			last_tag_samp = cur_tag_samp;					//update last_tag_samp with sample # from lastest tag
			last_tag_time = cur_tag_time;					//update last_tag_time with time from lastest tag
			
			std::cout<<"Updated Current Count to Reflect Errors: " << samp_cnt << std::endl << std::endl;
			
			tag_grp_cnt++;
			
		}//end if d_tags.size > 0 

		for (int i=0; i<noutput_items; i++){
			if (i < 10)
		//		std::cout << std::setw(12) << "Chunk Count: " << chunk_cnt << std::setw(12) << " Starting Sample Count: " << samp_cnt << std::endl << "Processing Data" << std::endl;
				
			out[i] = in[i];
			samp_cnt++;
		}//end for loop
//	uhd::time_spec_t tpps = device -> get_time_last_pps(0);
//		float tpps_s = tpps.get_real_secs();
	//std::cout << "Ending Sample Count: " << samp_cnt << std::endl;
//	std::cout << "Time last pps: " << tpps_s << std::endl;
	chunk_cnt++;
		
	}//end if
    // Tell runtime system how many output items we produced.
    return noutput_items;
        
} /* Work Function */
} /* namespace MyModules3 */
} /* namespace gr */

