/* 
* Copyright (C) 2007, Brian Tanner
* 
http://rl-glue.googlecode.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

package rlglue.types;

public class Action extends RL_abstract_type
{
	public Action() 
	{
		this(0,0);
	}

	public Action(int numInts, int numDoubles)
	{
		super(numInts,numDoubles);
	}
        
	public Action(Action src)
	{
		super(src);
	}
        

        public Action duplicate(){
            return new Action(this);
        }
}
