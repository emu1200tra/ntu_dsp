import speech_recognition as sr
import subprocess
from gtts import gTTS
from w2k import w2k

r = sr.Recognizer()
while True:
  with sr.Microphone() as source:  
    print "Please wait. Remind that you have 5 sec to record your query."  
    r.adjust_for_ambient_noise(source, duration=5)  
    print "Start record. Please say something."
    audio = r.listen(source) 
    print "done record"
   
  try:
    string = r.recognize_google(audio)
    print "Recognition :" + string

  except sr.UnknownValueError:
    print "Recognition falls. Can't understand query."
    continue
  except sr.RequestError as e:
    print "Could not request results ; {0}".format(e)
    break

  if string == "exit":
    break
  string = string.split()
  tmp = []
  for i in string:
    try:  
      tmp.append(w2k(i))
    except:
      tmp.append(i)
  string = ' '.join(tmp)
  
  tts = gTTS(text=string, lang='ja')
  tts.save("speech.mp3")
  subprocess.call(["ffmpeg" , "-i" , "speech.mp3" , "tmp.wav"])
  print "Recognition :" + string
  while True:
    try:
      subprocess.call(["play" , "tmp.wav"])
      break
    except:
      print "device busy , try again !"

  subprocess.call(['rm', 'speech.mp3' , 'tmp.wav'])
  print "start new record"

print "shut down successfully"