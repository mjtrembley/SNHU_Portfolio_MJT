 Coursework from CS305
 ---------------------
 Submitted by me, Matthew Trembley, for Southern New Hampshire University
 -----------------
 
 Briefly summarize your client, Artemis Financial, and their software requirements. Who was the client? What issue did they want you to address?

Artemis Financial is a fictional finance consultancy firm that develops individualized financial plans for savings, retirement, investments, and insurance for their clients.
They were concerned about security, and reached out to Global Rain, another fictional software company I "work" for, to modernize its operations and implment current and effective software security. They already had a RESTful web API, but they wanted Global Rain's expertise to improve upon security.
-------------------
What did you do particularly well in identifying their software security vulnerabilities? Why is it important to code securely? What value does software security add to a company’s overall wellbeing?

I felt what I did particularly well is analyzing the vulnerabilty report on the current API that was supplied to us. It seemed that most databases/libraries were out-of-date, so a seemingly simple fix. It is imporant for this to be coded securely because not only is it private peoples' information on the line, but the buisness as well. Software security adds toa  company's overall wellbeing by giving the sense of protection, and knowing they are delivering secure software to their end-users.

--------------------
What about the process of working through the vulnerability assessment did you find challenging or helpful?

The most helpful thing was how the report was generated. False positives were pretty easy to point-out, which made it a bit more confidence boosting. The challenging part was trying not to get lost in the extremely long dependency names. A lot of them seem similar, but are not.

--------------------
How did you approach the need to increase layers of security? What techniques or strategies would you use in the future to assess vulnerabilities and determine mitigation techniques?

The approach was the vulnerability assessment. It was single handedly the best way to go about it (in my limited knowledge), and it would be ideal for me to continue using this dependency check in the future.

-------------------
How did you ensure the code and software application were functional and secure? After refactoring code, how did you check to see whether you introduced new vulnerabilities?
What resources, tools, or coding practices did you employ that you might find helpful in future assignments or tasks?

Stack overflow was my saving grace. Trying to figure out how to code the SHA-256 cypher algorithm was a bit difficult, but I was able to successfully do it. Stack overflow has been a huge help over the CS program at SNHU, and sometimes I wish there was a SNHU specific spot for the same idea.
I checked to new vulnerabilities by running the code again through the dependency check. That way, I knew there was a before and after so if there were any new ones, I would know.

---------------------
Employers sometimes ask for examples of work that you have successfully completed to demonstrate your skills, knowledge, and experience. What from this particular assignment might you want to showcase to a future employer?

I would most likely showcase my use of the SHA-256 cypher algorithm. I don't feel like it is a very simple algorithm to use, but it is effective and I feel would speak loudly for what I have learned in this course.
