data0=""
data1=""
data2=""
data3=""
updateCount = 0

while (True):
        updateCount+=1
        print "Data" , data2 , ", x: " , x
        time.sleep(0.20)

        # Clear image buffer by drawing a black filled box.
        draw.rectangle((0,0,width,height), outline=0, fill=0)

        #get data
        if(updateCount > 10):
                f1 = os.popen('./mpdGetInfo | grep "artist: " | sed  "s/artist: //g"')
                f2 = os.popen('./mpdGetInfo | grep "title: " | sed  "s/title: //g"')
                f3 = os.popen('./mpdGetInfo | grep "album: " | sed  "s/album: //g"')
                f0 = os.popen('./mpdGetInfo | grep "elaspedTime: " | sed  "s/elaspedTime: //g"')
                data1 = f1.read()
                data2 = f2.read()
                data3 = f3.read()
                data0 = f0.read()

                updateCount = 0

        if( not data0):
                fdata0 = 0
        else:
                fdata0 = float(data0)

        # if just started track, then show at pos +60
        if(fdata0 < 10): x = -60
        #print "fdata3:",fdata3

        # show it
        draw.text((0, top), data1, font=font, fill=255)
        draw.text((-x, top+10), data2, font=font, fill=255)
        draw.text((0, top+20), data3, font=font, fill=255)


        #Draw the image buffer
        disp.image(image)
        disp.display()

        x = x + 5
        if (x > width):
                x = -width


print "All Done"
