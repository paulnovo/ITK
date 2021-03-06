/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkXMLFilterWatcher_h
#define __itkXMLFilterWatcher_h

#include "itkSimpleFilterWatcher.h"

namespace itk
{
/** \class XMLFilterWatcher
 * \brief Simple mechanism for monitoring the pipeline events of a
 * filter and reporting these events to std::cout. Formats reports
 * with xml.
 * \ingroup ITKCommon
 */
class XMLFilterWatcher:public SimpleFilterWatcher
{
public:
  XMLFilterWatcher(ProcessObject *o, const char *comment = ""):
    SimpleFilterWatcher(o, comment) {}
protected:

  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress()
  {
    if ( this->GetProcess() )
      {
      int steps = this->GetSteps();
      steps++;
      this->SetSteps(steps);
      if ( !this->GetQuiet() )
        {
        std::cout << "<filter-progress>"
                  << this->GetProcess()->GetProgress()
                  << "</filter-progress>"
                  << std::endl;
        std::cout << std::flush;
        }
      }
  }

  /** Callback method to show the StartEvent */
  virtual void StartFilter()
  {
    this->SetSteps(0);
    this->SetIterations(0);
    this->GetTimeProbe().Start();
    if ( !this->GetQuiet() )
      {
      std::cout << "<filter-start>"
                << std::endl;
      std::cout << "<filter-name>"
                << ( this->GetProcess()
           ? this->GetProcess()->GetNameOfClass() : "None" )
                << "</filter-name>"
                << std::endl;
      std::cout << "<filter-comment>"
                << " \"" << this->GetComment() << "\" "
                << "</filter-comment>"
                << std::endl;
      std::cout << "</filter-start>"
                << std::endl;
      std::cout << std::flush;
      }
  }

  /** Callback method to show the EndEvent */
  virtual void EndFilter()
  {
#if 0
    this - GetTimeProbe().Stop();
    if ( !this->GetQuiet() )
      {
      std::cout << "<filter-end>"
                << std::endl;
      std::cout << "<filter-name>"
                << ( this->GetProcess()
           ? this->GetProcess()->GetNameOfClass() : "None" )
                << "</filter-name>"
                << std::endl;
      std::cout << "<filter-time>"
                << this->GetTimeProbe().GetMeanTime()
                << "</filter-time>"
                << std::endl;
      std::cout << "</filter-end>";
      std::cout << std::flush;
      }
#endif
  }
};
} // end namespace itk

#endif
