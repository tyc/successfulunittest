# Introduction

## Why am I writing this book

I graduated from the Royal Melbourne Institute of Technology in 1992. Since then I have been working with embedded systems. Most engineering shops practicing this specialised artform are small to medium sized. Their engineering teams are usually small but resource hungry.

My first engineering team was a small team of three engineers. I was a graduate engineer, tasked with writing code for an embedded system. It was for an 8051 derivative from Philips controlling a vacuum replenishing system. At that time, the Australian government just passed a law banning ozone destroying gases, especially from the car’s air conditioner system. So this product would suck out the banned gas in the AC and pump the approved gas into it.

The small engineering team did a back of an envelope design. We coded it up, did some rudimentary testing and shipped it. The project struggled with all sorts of problems. The software was buggy at all different levels. It caused the delivery to be late and over budget. The project finally shipped, but with plenty of bugs and we lost a customers as result. It was a depressing lesson for me.

From that disastrous first project, and the many projects that followed it, I learned to ship a project on time, on budget and on spec. Careful planning must be ensured. A robust testing strategy must be part of that plan. One of the corner stones of the testing strategy is the unit test. With a good unit test plan, the whole project still has the potential to fall down, but that potential is much less.

I usually think of about this as getting all the small bricks that are part of a three storey house correct. The right type of bricks must be specified, and there must be test results that the bricks meet the specifications. Once satisfied with the results, I can safely proceed to build the house. If the bricks are not properly specified or tested, the house has the potential to collapse as the bricks are overstressed and will disintegrate.

In writing this book, I have hope to close the gap of providing practical ideas to successful unit tests for your code.  

## Who should read this book

* **The graduate software development engineer**
    - The graduate engineer will see how a unit test will increase the robustness of code that was created. Firstly it will take the graduate engineer from designing to structuring the code for testability. Secondly, it will show the graduate engineer how to collect the metrics to prove that the code is correctly implemented. This book will contain the necessary details needed for the graduate engineer to ensure that the software module is complete when it is delivered for integration.
* **The seasoned software engineer**
    - This book will cover topics which a seasoned software engineer should be familiar with. This book is ordered in a typical process with which the software module is tested and formalises the typical development process for a software module. It will discuss advance concepts such as traceability and test planning. 
* **The software team leader**
    - This book will show the software team leader what indicators to measure to ensure that the software modules are written to the necessary quality level. The signs will include the basic measures and items for review. The software team leader will take these metrics to provide a picture of the health of the overall software delivery. 
* **The quality assurance engineer**
    - This book will give the quality assurance engineer methods which can be used to monitor quality and derive measurements from the quality of the unit test specifications and the unit test report.

## I am assuming this about you

When writing this book, I am assuming you have been exposed to embedded systems and that you have some idea about the basic concepts of developing software for the embedded system. The concept of writing a device driver for the I2C peripheral means something to you, or at the very least, you are curious enough to find out more information.

You are curious and open to read about ideas and concepts new to you. You are curious enough to take the concepts presented in this book and to further extend them to suit your use case. Your use case will differ from the ones shown in this book. You are willing to experiment and willing to make plenty of mistakes.

You have worked in a team environment where your software modules are expected to be integrated into overall software with other modules. Your software module is also dependent upon other software modules.

## How to use this book

I have presented the information in this book in about the same order which a software is typically tested. However, it does not have to be read in this manner. You could easily have opened the book at any particular chapter to get the necessary info. Each chapter is self contained.

You should make your own annotation so that the concepts are understandable to you. If you have a physical copy of this book, I encourage you to pen down your own notes on the margins of the book. If reading this on a kindle, use the notes feature to annotate it. 

This book is a guide. It is not a text book that shows everything about unit tests and how it can be applied. It is shows you the basic idea of how a unit test is constructed and how it can be use to deliver robust code.

## Thank you

Finally, before we get into the book proper, I want to thank you for taking the time to learn about unit testing. I hope that you have got what are seeking from this book and can spread the word about the importance of early automated testing. 
