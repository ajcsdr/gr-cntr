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


#ifndef INCLUDED_MYMODULES3_MAGIC_H
#define INCLUDED_MYMODULES3_MAGIC_H

#include <MyModules3/api.h>
#include <gr_sync_block.h>

namespace gr {
  namespace MyModules3 {

    /*!
     * \brief <+description of block+>
     * \ingroup MyModules3
     *
     */
    class MYMODULES3_API magic : virtual public gr_sync_block
    {
    public:
       typedef boost::shared_ptr<magic> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of MyModules3::magic.
        *
        * To avoid accidental use of raw pointers, MyModules3::magic's
        * constructor is in a private implementation
        * class. MyModules3::magic::make is the public interface for
        * creating new instances.
        */
       static sptr make(float sample_rate);
       
      //virtual void set_sample_rate(float sample_rate) = 0;
    };

  } // namespace MyModules3
} // namespace gr

#endif /* INCLUDED_MYMODULES3_MAGIC_H */

