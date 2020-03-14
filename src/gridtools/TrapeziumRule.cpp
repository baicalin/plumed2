/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2016-2018 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed.org for more information.

   This file is part of plumed, version 2.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#include "core/ActionRegister.h"
#include "ActionWithIntegral.h"

//+PLUMEDOC GRIDANALYSIS TRAPEZIUM_RULE 
/*
Calculate the total integral of the function on the input grid

\par Examples

*/
//+ENDPLUMEDOC

namespace PLMD {
namespace gridtools {

class TrapeziumRule : public ActionWithIntegral {
public:
  static void registerKeywords( Keywords& keys );
  explicit TrapeziumRule(const ActionOptions&ao);
  void performTask( const unsigned& current, MultiValue& myvals ) const ;
};

PLUMED_REGISTER_ACTION(TrapeziumRule,"TRAPEZIUM_RULE")

void TrapeziumRule::registerKeywords( Keywords& keys ) {
  ActionWithIntegral::registerKeywords( keys );
}

TrapeziumRule::TrapeziumRule(const ActionOptions&ao):
  Action(ao),
  ActionWithIntegral(ao)
{
  if( getPntrToArgument(0)->getRank()!=1 ) error("can only use trapezium rule to integrate one dimensional functions");
}

void TrapeziumRule::performTask( const unsigned& current, MultiValue& myvals ) const {
  if( current==getFullNumberOfTasks()-1 ) return ;

  myvals.setValue( getPntrToOutput(0)->getPositionInStream(), 0.5*getVolume()*( getFunctionValue( current ) + getFunctionValue(current+1) ) );
  if( !doNotCalculateDerivatives() ) {
    myvals.addDerivative( getPntrToOutput(0)->getPositionInStream(), current, 0.5*getVolume() );
    myvals.updateIndex( getPntrToOutput(0)->getPositionInStream(), current );
    myvals.addDerivative( getPntrToOutput(0)->getPositionInStream(), current+1, 0.5*getVolume() );
    myvals.updateIndex( getPntrToOutput(0)->getPositionInStream(), current+1 );
  }
}

}
}