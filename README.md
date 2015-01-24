SimpleSpider
============
一、功能分析
-------
	这个使用C语言实现了一个基本的网络爬虫，目前（2015年1月24日）为止，主要设计的内容有链接分析、网页抓取。程序使用libevent（2.0.21-stable）来进行socket连接处理，使用自动机进行链接分析（详情见下文）。
	本程序要求：
	* 从给定网址作为入口，爬取对应入口网页并分析其中的URL并爬取对应的网页，直到没有更多页面爬取时完成
	* 爬取时只爬取本地服务器的网页，不怕取外站资源
	* 采用多线程结构设计，输出每一个文件的文件大小

	使用方法:
	* `make`
	* `./crawler entrance_url out_put_file`

