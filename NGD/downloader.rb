#@author waleed tuffaha
require 'rubygems'
require 'net/http'
require 'uri'

cache = {}
threads = []
to_get = File.readlines(ARGV[0])

f = File.open('output', 'w')

puts "Need to get #{to_get.length} queries ..!!"
start_time = Time.now

500.times do
  threads << Thread.new do
    while q_word = to_get.pop
      puts "remaining #{to_get.length} words" if to_get.length % 50 == 0
      if !cache[q_word]
        q_parsed = URI.parse(q_word)
        if res = Net::HTTP.get(q_parsed.host, q_parsed.request_uri).match(/About ([\d+,*]+?) results/)
            tmp = res.captures.first.gsub(",","")
            cache[q_word] = tmp
            f.flock(File::LOCK_EX)
            f.puts "#{q_word.chop},#{tmp}\n"
            f.flock(File::LOCK_UN)
        end
        end
      end
    end 
end


threads.each { |x| x.join }
puts "finished in #{Time.now - start_time}"
puts "writing output ..."
f.close()
puts "Done."
