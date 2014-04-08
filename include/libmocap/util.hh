// Copyright (c) 2014, CNRS-AIST JRL/UMI3218
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef LIBMOCAP_MARKER_UTIL_HH
# define LIBMOCAP_MARKER_UTIL_HH

# define LIBMOCAP_LVALUE_ACCESSOR(ATTR, TYPE)	\
  const TYPE& ATTR () const			\
  {						\
    return ATTR##_;				\
  }						\
  struct e_n_d__w_i_t_h__s_e_m_i__c_o_l_o_n

# define LIBMOCAP_RVALUE_ACCESSOR(ATTR, TYPE)	\
  TYPE& ATTR ()					\
  {						\
    return ATTR##_;				\
  }						\
  struct e_n_d__w_i_t_h__s_e_m_i__c_o_l_o_n

# define LIBMOCAP_ACCESSOR(ATTR, TYPE)		\
  LIBMOCAP_LVALUE_ACCESSOR (ATTR, TYPE);	\
  LIBMOCAP_RVALUE_ACCESSOR (ATTR, TYPE);	\
  struct e_n_d__w_i_t_h__s_e_m_i__c_o_l_o_n

#endif //! LIBMOCAP_MARKER_UTIL_HH
