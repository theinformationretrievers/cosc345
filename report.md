# Intro matt

For our project we've decided to build a text reader with a Maori vocabulary training component. It will allow users to open a variety of text file formats (txt, ePub etc) and it will replace a portion of the words with their Maori translation with the facility to hover over the word to see the original. It will have a library of public domain eBooks for users to read. We see this as an option to replace the users default text reader, something akin to adobe pff reader but for all text. 

The software will come with extended learning features included, and will utitlize the diglot weave approach which involves reading text in the users native language then slowly replacing words with the language which they wish to learn. In this case the focus will be on replacing head words with their maori counterparts. It will cycle words in and out to increase the vocabulary retention, and have the ability to mark words as learned. The app will provide the ability to swap out the dictionary mappings to different languages etc english to spanish. 



Our team consists of Jet, Jake, Matt & Cody ***Experience of each team member? ***

Jake

I am a third year computer science student with interest in low level programming and design. I have some experience in natural language processing. I am well practiced in working with teams from my time as a chef, however this is the first time building an app as a team from scratch.   '

Jet

I'm Jet. I am almost finished undergrad computer science and am starting honours next year. I have experience with C++ in the context of graphics and computer vision. In addition to this I have designs skills and some artistic ability which I think will be valuable for this project.

Cody

Third year computer science student with an information science minor, including 4 years of experience in IT. This is my third team based software project. I have experience in NLP, doing research over the summer, which should hopefully prove useful for this project.

# What datasets are we using?

Explain our datasets, why we chose them, how they combine and how that combination benefits the application. 

# How will we build it? 

Can add diagram of what I think it'll look like. Mention C++, Ultralight etc as possible technologies. 

Can mention the challenges we need to overcome to have a working app. Can discuss  
Look at roles our team members will take. Talk about Agileish (with that hint of waterfall) which will lead into our schedule.
We have made a rough diagram to show the architecture for the project

![diagram](./mockups/diagram.png)  

As seen in the diagram we are trying to split the application into three main areas:

- Backend API
- Controller
- GUI

### Backend API

This is further split into three different sections:

- User Profile
- Translator
- Substitute

We aim to keep each part of the backend isolated, in that it will send and recieve data through the controller abstraction layer

### Controller

This will handle the data going between different areas of the code. The GUI will send text and any updates to user data to the controller, which will then make the API calls to coordinate the backend. Once the backend has finished its computation, the controller will then send the result to the GUI. 

### GUI

From a UI design perspective we are aiming to have a low clutter and simplistic design aimed to be easy on the eyes for long reading sessions. We have made some mockups in figma of some of the features that we envision for the app. These mockups will then be transferred to html and css using the ultralight framework as a html renderer. Figma itself allows for the designs to be exported to html and css, though there will also be some clean up on our end. Ultralight allows us to make an app using a UI framework we understand, html, but be able to easily couple it with C++. 

![landing_page](./mockups/landing_page.png) 
![reader](./mockups/reader.png)  


As seen we will have a landing page from which you can open recent books or documents. Additionally there is a sidebar which will show a typical file system to navigate to files, or optionally look through the users library. 



# What is our schedule? cody

Our sprints are separated into our milestones, leaving us with 4 weeks for the alpha, 2 for the beta, and 3 for the final product.

The Alpha

The alpha minimal viable product involves three main parts: Data organization, Frontend setup for the template of our app, and the implementation of the translation + controller architecture. This leaves us with an app that can switch out english words for maori, no more no less.

![gantt_chart](mockups/gantt_chart.png)


Gantt charts. Include hard deadlines such as assignments and end of semester + mid sem breaks. 

# What apps already exist?

look at current companies, products etc. 

# Is there interest in our app matt

Look at New Zealand's propensity to use Maori loan words.

# Summary

 