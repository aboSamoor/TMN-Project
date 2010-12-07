require 'rubygems'
require 'net/http'

cache = {}
threads = []
to_get = File.readlines(ARGV[0])

f = File.open('output', 'w')

puts "Need to get #{to_get.length} queries ..!!"
start_time = Time.now

1000.times do
  threads << Thread.new do
    while q_word = to_get.pop
      puts "remaining #{to_get.length} words" if to_get.length % 50 == 0
      if !cache[q_word]
        res = false
        while not res
            if res = Net::HTTP.get('www.google.com', "/search?q=#{q_word}").match(/About ([\d+,*]+?) results/)
                cache[q_word] = res.captures.first
                f.puts "#{q_word},#{res.captures.first}"
            end
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
