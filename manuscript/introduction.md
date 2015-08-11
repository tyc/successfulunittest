# Introduction

## Why am I writing this book

Ever since I have graduated from the Royal Melbourne Institute of Technology in 1992, I have been working with embedded systems. Living in Australia at the time, most engineering shop practicing in this form of specialise engineering artform are small to medium size. Their engineering teams are usually small and resource hungry.

My first engineering team was a small team of three engineers. I was a graduate engineer, tasked with writing code for an embedded system. It was for an 8051 derivative from Philips. It was controlling a vacuum replenishing system. At that time, the Australian government just passed a law banning ozone destroying gases in the car's air conditioner system. So this product would suck out the banned gas in air conditioner and pump the approved gas into it. 

The small engineering team did a very "back of an envelope" design, coded it up, did some rudimentary testing and shipped it. The project had all sorts of problems with it. It caused the delivery to be late and over budget. The project finally shipped, but with plenty of bugs and we lost a customer as result of it. Personally, it was a depressing lesson for me.

From that disastarous first project and the many projects that followed it, I learned to shipped a project on time, on budget and on spec, careful planning must ensure. A robust testing strategy must be part of that plan. One of the corner stones of the testing strategy is the unit test. With a good unit test plan, the whole project will still has the potential to fall down but that potential is much less.

I usually think of about this as a getting all the small bricks that are part of a three storey house correct. The right type of bricks must be specified, and there must be test results that the bricks meet the specifications. Once I am satisfy with the results, then I can safely proceed to build the house. If the bricks are not properly specified or tested, the house has the potential to collapse as the bricks is overstressed and will disintegrate.

In writing this book, I have hope to close the gap of providing practical ideas to sucessful unit tests for your code.  

## Who should read this book

* **The graduate software development engineer**
    - The graduate engineer will see how a unit test will increase the robustness of code that was created. Firstly it will take the graduate engineer how to design and structure the code for testability. Secondly, it will show the graduate engineer how to collect the metrics to prove that the code is correctly implemented. This book will contain the necessary details needed for the graduate engineer to ensure that the software module is complete when it is delivered for intergration.
* **The seasoned software engineer**
    - This book will go over grounds which a seasoned software engineer should be familiar with. This book is ordered in a typical process which the software module is tested. From a seasoned software engineer, this book formalises the typical development process for a software module.
* **The software team leader**
    - This book will show the software team leader what indicators to measure to ensure that the software modules are written to the necessary quality level. The signs will include the basic measures and items for review. The software team leader will take these metrics to provide a picture of the health of the overall software delivery. 
* **The quality assurance engineer**
    - This book will give the quality assurance engineer methods which can be used to monitor the quality of the software. The quality of the software also includes measures from the quality of the unit test specifications and the unit test report.

## I am assuming this about you

When I am writing this book, I am assuming that you have been exposed to the embedded system and that you have some idea to the basic concepts of developing software for the embedded system.

You are curious and open to read about ideas and concepts new to you. You are curious enough to take the concepts presented in this book and to further extend them to suit your use case. Your use case will unlikely to be exactly the same as that shown in this book. You are willing to experiment and willing to make plenty of mistakes.

You have worked in a team environment where the your software modules are expected to be integrated into overall software with other software modules. Your software module is also dependent upon other software modules as well.

## How to use this book

I have presented the information in this book in about the same order which a software is typically tested. However, it does not have to be read in this manner. You could easily have opened the book to a particular chapter to get the necessary. Each chapter is self contain.

I would encourage you to take a pencil or a pen, and make your own notes on margins of the book. Make your own annotation so that the concepts are understandable to you.

## Thank you

Finally, before we get into the book proper. I want to thank you for taking the time to purchase this book. I hope that you have got what are seeking from this book. 

