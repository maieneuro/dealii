//----------------------------  trilinos_block_sparse_matrix_vector_04.cc  ---------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2013 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  trilinos_block_sparse_matrix_vector_04.cc  ---------------------------


// check BlockSparseMatrix::vmult, Tvmult with deal.II vector/vector
// combination

#include "../tests.h"
#include <deal.II/base/utilities.h>
#include <deal.II/lac/block_vector.h>
#include <deal.II/lac/trilinos_block_sparse_matrix.h>
#include <fstream>
#include <iostream>
#include <vector>


void test (const unsigned int size_1, const unsigned int size_2)
{
  TrilinosWrappers::SparseMatrix m(size_1, size_2, size_2);
  for (unsigned int i=0; i<m.m(); ++i)
    for (unsigned int j=0; j<m.n(); ++j)
        m.set (i,j, i+2*j);
  m.compress (VectorOperation::insert);

  TrilinosWrappers::BlockSparseMatrix m_block;
  m_block.reinit(1,1);
  m_block.block(0,0).copy_from(m);
  m_block.collect_sizes();

  Vector<double> w(size_1), v(size_2);
  for (unsigned int i=0; i<v.size(); ++i)
    v(i) = i;

                                   // w:=Mv
  m_block.vmult (w,v);

                                   // make sure we get the expected result
  for (unsigned int i=0; i<m.m(); ++i)
    {
      double result = 0;
      for (unsigned int j=0; j<m.n(); ++j)
        result += (i+2*j)*j;
      Assert (w(i) == result, ExcInternalError());
    }

  for (unsigned int i=0; i<w.size(); ++i)
    w(i) = i;

  m_block.Tvmult (v, w);
                                   // make sure we get the expected result
  for (unsigned int i=0; i<m.n(); ++i)
    {
      double result = 0;
      for (unsigned int j=0; j<m.m(); ++j)
        result += (j+2*i)*j;
      Assert (v(i) == result, ExcInternalError());
    }

  deallog << "OK" << std::endl;
}



int main (int argc, char **argv)
{
  std::ofstream logfile("block_sparse_matrix_vector_04/output");
  deallog.attach(logfile);
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  Utilities::MPI::MPI_InitFinalize mpi_initialization (argc, argv);


  try
    {
      test (50,47);
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      std::cerr << "Exception on processing: " << std::endl
		<< exc.what() << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      std::cerr << "Unknown exception!" << std::endl
		<< "Aborting!" << std::endl
		<< "----------------------------------------------------"
		<< std::endl;
      return 1;
    };
}