#@author waleed tuffaha
require 'rubygems'
require 'net/http'
require 'uri'

cache = {}
threads = []
to_get = File.readlines(ARGV[0])
dup = 0; dup_lock = Mutex.new

f = File.open('output', 'w')

puts "Need to get #{to_get.length} queries ..!!"
start_time = Time.now

1000.times do
  threads << Thread.new do
    while q_word = to_get.pop
      puts "remaining #{to_get.length} words" if to_get.length % 50 == 0
      if !cache[q_word]
        q_parsed = URI.parse(q_word)
        res_http = Net::HTTP.get(q_parsed.host, q_parsed.request_uri)
        result_stats = res_http.match(/<div[^>]*resultStats[^>]*>(.*?)<\/div>/)
        tmp = result_stats ? result_stats[1].match(/([\d+,]+)/)[1].gsub(",","") : 0
        cache[q_word] = tmp
        f.flock(File::LOCK_EX)
        f.puts "#{q_word.chop},#{tmp}\n"
        f.flock(File::LOCK_UN)
      else
        dup_lock.synchronize { dup += 1 }
      end
    end
  end 
end


threads.each { |x| x.join }
puts "finished in #{Time.now - start_time}"
puts "#{dup} duplicates found"
puts "writing output ..."
f.close()
puts "Done."

