/*

   two experiments should be run:
		 At each beat, the events up to and including 
  	 the next beat should be scheduled.
   or
  	 at each beat, the events up until the next beat
  	 should be scheduled. This includes events at the beat itself. 

	NEW IDEA:
		Each step has a length
		There no 'steps per beat' anymore
		each step plays for a division of a beat from 128 beats up to 1/16 of a beat (maybe 1/32?)
		Each sequence must be able to be 1024 beats long max
		Max 64 steps still stands
		
		There need to be two things that are kept track of for this:
			When the sequence starts and stops
			The length of each step stored in mcs in an array

		elapsedMicros sequenceTimer
		elapsedMicros stepTimer

		for a 16 beat sequence at 120bpm 
		when sequenceTimer reaches 8 seconds,
		sequenceTimer is reset to 0 and the sequence is restarted

		For each step, 
		when stepTimer reaches the length of time for the current step,
		stepTimer is reset to 0 and the next step is played
		if the last step is reached, the sequence should start again from the beginning
		it could also be reversed depending on sequence settings.

	Beat calculations should be executed before the note instructions are executed.

   Latency for each scenario should be measured
   Start / Stop should function as expected without too much
   extra calculations. 

 	 Simpler is better.
 	 Need to determine a way to keep time.
 	 Each trigger should be calculated in microseconds since the beat

 	 Also need to think about time drift. There should be a master clock
 	 that is reset to 0 when the sequence is restarted, or when the tempo is changed.
 	 this master clock should keep track of when the beats should be based on
 	 when the sequence was triggered, and ensure that what is playing reflects what 
 	 should be happening. If there is a drift, it should correct it.

 	 This master clock should also keep time with external midi sync, if present.
*/